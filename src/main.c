#include <direct.h>
#include <inttypes.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <process.h>
#endif

#include "ast.h"
#include "parser.h"
#include "token.h"

#ifndef ULTIMA_CLANG_PATH
#define ULTIMA_CLANG_PATH "C:\\LLVM-dev\\bin\\clang.exe"
#endif
#ifndef ULTIMA_RUNTIME_PATH
#define ULTIMA_RUNTIME_PATH "..\\src\\runtime.c"
#endif

static inline void verify_llvm(LLVMModuleRef mod) {
  char* error = NULL;
  if (LLVMVerifyModule(mod, LLVMAbortProcessAction, &error)) {
    fprintf(stderr, "LLVM verify error: %s\n", error);
    LLVMDisposeMessage(error);
    exit(1);
  }
}

struct Local {
  const char* name;
  LLVMValueRef alloca;
};

struct Codegen {
  LLVMModuleRef mod;
  LLVMBuilderRef builder;
  LLVMTypeRef i32;
  LLVMTypeRef i64;
  LLVMValueRef print_fn;
  LLVMTypeRef print_type;
  struct Local locals[256];
  int locals_count;
};

static LLVMValueRef locals_get(struct Codegen* cg, const char* name) {
  for (int i = cg->locals_count - 1; i >= 0; i--) {
    if (strcmp(cg->locals[i].name, name) == 0) return cg->locals[i].alloca;
  }
  return NULL;
}

static void locals_put(struct Codegen* cg, const char* name, LLVMValueRef alloca) {
  cg->locals[cg->locals_count].name = name;
  cg->locals[cg->locals_count].alloca = alloca;
  cg->locals_count++;
}

static LLVMValueRef codegen_expr(struct Codegen* cg, struct Ast* expr) {
  switch (expr->kind) {
    case AST_INT:
      return LLVMConstInt(cg->i32, (unsigned long long)expr->int_value, false);
    case AST_IDENT: {
      LLVMValueRef slot = locals_get(cg, expr->name);
      if (!slot) {
        fprintf(stderr, "unknown identifier: %s\n", expr->name);
        exit(1);
      }
      return LLVMBuildLoad2(cg->builder, cg->i32, slot, expr->name);
    }
    case AST_BINARY: {
      LLVMValueRef l = codegen_expr(cg, expr->binary.left);
      LLVMValueRef r = codegen_expr(cg, expr->binary.right);
      return LLVMBuildAdd(cg->builder, l, r, "addtmp");
    }
    default:
      fprintf(stderr, "expr kind not supported: %d\n", expr->kind);
      exit(1);
  }
}

static void codegen_stmt(struct Codegen* cg, struct Ast* stmt) {
  switch (stmt->kind) {
    case AST_VAR_DECL: {
      LLVMValueRef slot = LLVMBuildAlloca(cg->builder, cg->i32, stmt->var_decl.name);
      LLVMValueRef v = codegen_expr(cg, stmt->var_decl.value);
      LLVMBuildStore(cg->builder, v, slot);
      locals_put(cg, stmt->var_decl.name, slot);
    } break;

    case AST_CALL: {
      if (strcmp(stmt->call.callee, "print") != 0) {
        fprintf(stderr, "only rt_print_i32 supported for now, got: %s\n", stmt->call.callee);
        exit(1);
      }
      LLVMValueRef arg = codegen_expr(cg, stmt->call.arg);
      LLVMBuildCall2(cg->builder, cg->print_type, cg->print_fn, &arg, 1, "");
    } break;

    case AST_RETURN: {
      LLVMValueRef v32 = codegen_expr(cg, stmt->return_value);
      LLVMValueRef v64 = LLVMBuildZExt(cg->builder, v32, cg->i64, "ret64");
      LLVMBuildRet(cg->builder, v64);
    } break;

    default:
      fprintf(stderr, "stmt kind not supported: %d\n", stmt->kind);
      exit(1);
  }
}

static void codegen_function(struct Codegen* cg, struct Ast* fn) {
  if (fn->kind != AST_FUNCTION) {
    fprintf(stderr, "expected AST_FUNCTION\n");
    exit(1);
  }
  if (fn->function.body->kind != AST_BLOCK) {
    fprintf(stderr, "expected AST_BLOCK body\n");
    exit(1);
  }

  cg->locals_count = 0;

  LLVMTypeRef main_type = LLVMFunctionType(cg->i64, NULL, 0, false);
  LLVMValueRef main_fn = LLVMAddFunction(cg->mod, fn->function.name, main_type);

  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_fn, "entry");
  LLVMPositionBuilderAtEnd(cg->builder, entry);

  struct Ast* block = fn->function.body;
  for (int i = 0; i < block->block.count; i++) codegen_stmt(cg, block->block.statements[i]);

  if (!LLVMGetBasicBlockTerminator(entry)) LLVMBuildRet(cg->builder, LLVMConstInt(cg->i64, 0, false));
}

static void codegen_module_from_ast(LLVMModuleRef mod, struct Ast* program) {
  struct Codegen cg = {0};
  cg.mod = mod;
  cg.builder = LLVMCreateBuilder();
  cg.i32 = LLVMInt32Type();
  cg.i64 = LLVMInt64Type();

  LLVMTypeRef print_args[] = {cg.i32};
  cg.print_type = LLVMFunctionType(LLVMVoidType(), print_args, 1, false);
  cg.print_fn = LLVMAddFunction(mod, "rt_print_i32", cg.print_type);

  codegen_function(&cg, program);

  LLVMDisposeBuilder(cg.builder);
}

static inline void emit_object_file(LLVMModuleRef mod, const char* obj_path) {
  char* error = NULL;
  char* triple = LLVMGetDefaultTargetTriple();

  LLVMTargetRef target;
  if (LLVMGetTargetFromTriple(triple, &target, &error)) {
    fprintf(stderr, "Target error: %s\n", error);
    LLVMDisposeMessage(error);
    exit(1);
  }

  LLVMTargetMachineRef tm = LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault, LLVMRelocDefault, LLVMCodeModelDefault);
  LLVMSetTarget(mod, triple);

  if (LLVMTargetMachineEmitToFile(tm, mod, obj_path, LLVMObjectFile, &error)) {
    fprintf(stderr, "Emit error: %s\n", error);
    LLVMDisposeMessage(error);
    exit(1);
  }

  LLVMDisposeTargetMachine(tm);
  LLVMDisposeMessage(triple);
}

static inline char* read_file(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "Failed to open %s\n", path);
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char* buffer = malloc(size + 1);
  fread(buffer, 1, size, f);
  buffer[size] = 0;

  fclose(f);
  return buffer;
}

static inline const char* get_output_name(int argc, char** argv) {
  for (int i = 2; i + 1 < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) return argv[i + 1];
  }
  return "hello.exe";
}

static inline void link_executable(const char* clang_path, const char* obj_path, const char* runtime_c, const char* out_exe) {
#ifdef _WIN32
  const char* args[] = {clang_path, obj_path, runtime_c, "-o", out_exe, NULL};

  printf("Link: %s %s %s -o %s\n", clang_path, obj_path, runtime_c, out_exe);

  int rc = _spawnv(_P_WAIT, clang_path, args);
  if (rc != 0) {
    fprintf(stderr, "link failed (code=%d)\n", rc);
    exit(1);
  }
#else
  char cmd[4096];
  snprintf(cmd, sizeof(cmd), "\"%s\" \"%s\" \"%s\" -o \"%s\"", clang_path, obj_path, runtime_c, out_exe);
  printf("Link: %s\n", cmd);
  int rc = system(cmd);
  if (rc != 0) {
    fprintf(stderr, "link failed (code=%d)\n", rc);
    exit(1);
  }
#endif
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ultima <file.ult>\n");
    return 1;
  }

  const char* input_file = argv[1];
  const char* out_exe = get_output_name(argc, argv);

  char* source = read_file(input_file);

  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  LLVMModuleRef mod = LLVMModuleCreateWithName("ultima_module");

  struct Lexer l;
  init_lexer(&l, source);

  while (true) {
    struct Token t = next_token(&l);
    printf("Token kind: %d, text: %.*s\n", t.kind, t.length, t.start);
    if (t.kind == TOK_EOF)
      break;
  }

  struct Parser parser;
  parser_init(&parser, source);

  struct Ast* program = parse_program(&parser);
  codegen_module_from_ast(mod, program);

  char cwd[1024];
  _getcwd(cwd, sizeof(cwd));
  printf("CWD: %s\n", cwd);

  verify_llvm(mod);
  const char* obj_path = "main.obj";
  const char* runtime_path = ULTIMA_RUNTIME_PATH;
  emit_object_file(mod, obj_path);

  LLVMDisposeModule(mod);

  // Link step to turn obj in exe
  link_executable(ULTIMA_CLANG_PATH, obj_path, runtime_path, out_exe);

  printf("Built: %s\n", out_exe);

  return 0;
}
