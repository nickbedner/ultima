#include "token.h"

void init_lexer(struct Lexer* l, const char* source) {
  l->start = source;
  l->current = source;
}

static bool is_at_end(struct Lexer* l) {
  return *l->current == '\0';
}

static char advance(struct Lexer* l) {
  l->current++;
  return l->current[-1];
}

static char peek(struct Lexer* l) {
  return *l->current;
}

static void skip_whitespace(struct Lexer* l) {
  while (true) {
    char c = peek(l);
    if (c == ' ' || c == '\r' || c == '\t' || c == '\n')
      advance(l);
    else
      break;
  }
}

static struct Token make_token(struct Lexer* l, enum TokenKind kind) {
  struct Token t;
  t.kind = kind;
  t.start = l->start;
  t.length = (int)(l->current - l->start);
  return t;
}

static bool match_keyword(const char* start, int length, const char* keyword) {
  return strlen(keyword) == (size_t)length && strncmp(start, keyword, length) == 0;
}

static struct Token identifier(struct Lexer* l) {
  while (isalnum(peek(l)) || peek(l) == '_')
    advance(l);

  int length = (int)(l->current - l->start);

  if (match_keyword(l->start, length, "i32"))
    return make_token(l, TOK_I32);
  if (match_keyword(l->start, length, "i64"))
    return make_token(l, TOK_I64);
  if (match_keyword(l->start, length, "return"))
    return make_token(l, TOK_RETURN);

  return make_token(l, TOK_IDENT);
}

static struct Token number(struct Lexer* l) {
  while (isdigit(peek(l)))
    advance(l);
  return make_token(l, TOK_INT);
}

struct Token next_token(struct Lexer* l) {
  skip_whitespace(l);
  l->start = l->current;

  if (is_at_end(l))
    return make_token(l, TOK_EOF);

  char c = advance(l);

  if (isalpha(c) || c == '_')
    return identifier(l);
  if (isdigit(c))
    return number(l);

  switch (c) {
    case '(':
      return make_token(l, TOK_LPAREN);
    case ')':
      return make_token(l, TOK_RPAREN);
    case '{':
      return make_token(l, TOK_LBRACE);
    case '}':
      return make_token(l, TOK_RBRACE);
    case ';':
      return make_token(l, TOK_SEMI);
    case '=':
      return make_token(l, TOK_EQUAL);
    case '+':
      return make_token(l, TOK_PLUS);
  }

  printf("Unexpected character: %c\n", c);
  return make_token(l, TOK_EOF);
}