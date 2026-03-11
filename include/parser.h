#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "token.h"

struct Parser {
  struct Lexer lexer;
  struct Token current;
};

void parser_init(struct Parser* p, const char* source);
struct Ast* parse_program(struct Parser* p);
