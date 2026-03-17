#pragma once

enum ASTKind {
  AST_INT,
  AST_IDENT,
  AST_BINARY,
  AST_VAR_DECL,
  AST_CALL,
  AST_RETURN,
  AST_BLOCK,
  AST_FUNCTION
};

struct Ast {
  enum ASTKind kind;

  union {
    // AST_INT
    int int_value;

    // AST_IDENT
    char* name;

    // AST_BINARY
    struct {
      struct Ast* left;
      struct Ast* right;
    } binary;

    // AST_VAR_DECL
    struct {
      char* name;
      struct Ast* value;
    } var_decl;

    // AST_CALL
    struct {
      char* callee;
      struct Ast* arg;
    } call;

    // AST_RETURN
    struct Ast* return_value;

    // AST_BLOCK
    struct {
      struct Ast** statements;
      int count;
    } block;

    // AST_FUNCTION
    struct {
      char* name;
      struct Ast* body;
    } function;
  };
};
