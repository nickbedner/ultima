#include "parser.h"

static void error(const char* msg) {
  fprintf(stderr, "Parse error: %s\n", msg);
  exit(1);
}

static void advance(struct Parser* p) {
  p->current = next_token(&p->lexer);
}

static int match(struct Parser* p, enum TOKEN_KIND kind) {
  if (p->current.kind == kind) {
    advance(p);
    return 1;
  }

  return 0;
}

static void expect(struct Parser* p, enum TOKEN_KIND kind, const char* msg) {
  if (!match(p, kind)) error(msg);
}

static char* token_to_string(struct Token t) {
  char* s = malloc(t.length + 1);
  memcpy(s, t.start, t.length);
  s[t.length] = 0;

  return s;
}

void parser_init(struct Parser* p, const char* source) {
  init_lexer(&p->lexer, source);
  advance(p);
}

static struct Ast* parse_expression(struct Parser* p) {
  struct Ast* left;

  if (p->current.kind == TOK_INT) {
    left = malloc(sizeof(struct Ast));
    left->kind = AST_INT;
    left->int_value = atoi(token_to_string(p->current));
    advance(p);
  } else if (p->current.kind == TOK_IDENT) {
    left = malloc(sizeof(struct Ast));
    left->kind = AST_IDENT;
    left->name = token_to_string(p->current);
    advance(p);
  } else
    error("Expected expression");

  if (match(p, TOK_PLUS)) {
    struct Ast* right = parse_expression(p);

    struct Ast* node = malloc(sizeof(struct Ast));
    node->kind = AST_BINARY;
    node->binary.left = left;
    node->binary.right = right;
    return node;
  }

  return left;
}

static struct Ast* parse_statement(struct Parser* p) {
  // i32 var = expr;
  if (p->current.kind == TOK_I32) {
    advance(p);

    if (p->current.kind != TOK_IDENT)
      error("Expected identifier");

    char* name = token_to_string(p->current);
    advance(p);

    expect(p, TOK_EQUAL, "Expected '='");

    struct Ast* value = parse_expression(p);
    expect(p, TOK_SEMI, "Expected ';'");

    struct Ast* node = malloc(sizeof(struct Ast));
    node->kind = AST_VAR_DECL;
    node->var_decl.name = name;
    node->var_decl.value = value;

    return node;
  }

  // return expr;
  if (p->current.kind == TOK_RETURN) {
    advance(p);

    struct Ast* value = parse_expression(p);
    expect(p, TOK_SEMI, "Expected ';'");

    struct Ast* node = malloc(sizeof(struct Ast));
    node->kind = AST_RETURN;
    node->return_value = value;

    return node;
  }

  // function call
  if (p->current.kind == TOK_IDENT) {
    char* callee = token_to_string(p->current);
    advance(p);

    expect(p, TOK_LPAREN, "Expected '('");
    struct Ast* arg = parse_expression(p);
    expect(p, TOK_RPAREN, "Expected ')'");
    expect(p, TOK_SEMI, "Expected ';'");

    struct Ast* node = malloc(sizeof(struct Ast));
    node->kind = AST_CALL;
    node->call.callee = callee;
    node->call.arg = arg;

    return node;
  }

  error("Unknown statement");
  return NULL;
}

static struct Ast* parse_block(struct Parser* p) {
  expect(p, TOK_LBRACE, "Expected '{'");

  struct Ast** statements = malloc(sizeof(struct Ast*) * 256);
  int count = 0;

  while (p->current.kind != TOK_RBRACE)
    statements[count++] = parse_statement(p);

  expect(p, TOK_RBRACE, "Expected '}'");

  struct Ast* block = malloc(sizeof(struct Ast));
  block->kind = AST_BLOCK;
  block->block.statements = statements;
  block->block.count = count;

  return block;
}

static struct Ast* parse_function(struct Parser* p) {
  if (p->current.kind != TOK_I64)
    error("Expected i64");

  advance(p);

  if (p->current.kind != TOK_IDENT)
    error("Expected function name");

  char* name = token_to_string(p->current);
  advance(p);

  expect(p, TOK_LPAREN, "Expected '('");
  expect(p, TOK_RPAREN, "Expected ')'");
  struct Ast* body = parse_block(p);

  struct Ast* fn = malloc(sizeof(struct Ast));
  fn->kind = AST_FUNCTION;
  fn->function.name = name;
  fn->function.body = body;

  return fn;
}

struct Ast* parse_program(struct Parser* p) {
  return parse_function(p);
}
