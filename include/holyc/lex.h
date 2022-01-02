#ifndef HOLYC_SRC_LEX
#define HOLYC_SRC_LEX

#include <holyc/cc.h>

#include <stdio.h>
#include <stdlib.h>

void error_at(CC *cc);

typedef enum {
  // TK_ERROR,
  TK_EOF,
  TK_INT,
  TK_MIN,
  TK_PLUS,
  TK_DIV,
  TK_MUL,
  TK_LPAREN,
  TK_RPAREN,
  TK_SEMI,
} TokenType;

// Print a token for debugging.
//
void print_token(CC *cc);

// Fetches next token.
//
int Lex(CC *cc);

// Unfetches next token. HACKY
//
void Unlex(CC *cc);

#endif // HOLYC_SRC_LEX
