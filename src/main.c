#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>

static inline void verify_llvm(LLVMModuleRef module) {
  char *error = NULL;
  LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
  // if (error != NULL && error[0] != 0 && error[0] != '\0') {
  //   fprintf(stderr, "error: %s\n", error);
  //   LLVMDisposeMessage(error);
  //   exit(EXIT_FAILURE);
  // }
  LLVMDisposeMessage(error);
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  printf("Hello world!\n");

  LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");

  //
  LLVMTypeRef param_types[] = {LLVMInt32Type(), LLVMInt32Type()};
  // LLVMTypeRef ret_type = LLVMFunctionType(LLVMInt32Type(), param_types, 2, 0);
  // LLVMValueRef sum = LLVMAddFunction(mod, "sum", ret_type);
  // LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sum, "entry");
  LLVMBuilderRef builder = LLVMCreateBuilder();
  // LLVMPositionBuilderAtEnd(builder, entry);
  // LLVMValueRef tmp = LLVMBuildAdd(builder, LLVMGetParam(sum, 0), LLVMGetParam(sum, 1), "tmp");
  // LLVMBuildRet(builder, tmp);
  //
  // char *error = NULL;
  // LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
  // LLVMDisposeMessage(error);
  //
  LLVMTypeRef int_8_type = LLVMInt8Type();
  LLVMTypeRef int_8_type_ptr = LLVMPointerType(int_8_type, 0);
  LLVMTypeRef int_32_type = LLVMInt32Type();
  LLVMTypeRef puts_function_args_type[] = {int_8_type_ptr};
  LLVMTypeRef puts_function_type = LLVMFunctionType(int_32_type, puts_function_args_type, 1, false);
  LLVMValueRef puts_function = LLVMAddFunction(mod, "puts", puts_function_type);
  // LLVMBasicBlockRef entry2 = LLVMAppendBasicBlock(puts_function, "entry2");
  // LLVMPositionBuilderAtEnd(builder, entry2);
  // LLVMValueRef tmp2 = LLVMBuildCall(builder, puts_function, puts_function_args, 1, "putscall");
  //  LLVMBuildRet(builder, tmp2);
  // LLVMBuildRet(builder, LLVMConstInt(int_32_type, 0, false));
  //
  LLVMTypeRef main_function_type = LLVMFunctionType(int_32_type, param_types, 2, false);
  LLVMValueRef main_function = LLVMAddFunction(mod, "main", main_function_type);
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(main_function, "entry");
  LLVMPositionBuilderAtEnd(builder, entry);
  LLVMValueRef puts_function_args[] = {LLVMBuildPointerCast(builder, LLVMBuildGlobalString(builder, "Hello, World!", "hello"), int_8_type_ptr, "0")};
  LLVMBuildCall2(builder, puts_function_type, puts_function, puts_function_args, 1, "i");
  LLVMBuildRet(builder, LLVMConstInt(int_32_type, 0, false));

  char *error = NULL;
  LLVMVerifyModule(mod, LLVMAbortProcessAction, &error);
  LLVMDisposeMessage(error);

  // Write out bitcode to file
  if (LLVMWriteBitcodeToFile(mod, "sum.bc") != 0) {
    fprintf(stderr, "error writing bitcode to file, skipping\n");
  }
  LLVMPrintModuleToFile(mod, "hello.ll", 0);

  LLVMDisposeBuilder(builder);

  return 0;
}
