#pragma once

#include <ctype.h>
#include <stdio.h>
#include <string.h>

enum TOKEN_KIND {
  TOK_INT,
  TOK_IDENT,
  TOK_I32,
  TOK_I64,
  TOK_RETURN,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_SEMI,
  TOK_EQUAL,
  TOK_PLUS,
  TOK_EOF
};

struct Token {
  enum TOKEN_KIND kind;
  const char* start;
  int length;
};

struct Lexer {
  const char* start;
  const char* current;
};

void init_lexer(struct Lexer* l, const char* source);
struct Token next_token(struct Lexer* l);
