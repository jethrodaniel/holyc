#ifndef HOLYC_LEX
#define HOLYC_LEX

#include <holyc/buffer.h>

#include <stdbool.h>

typedef enum {
  TK_UNINITIALIZED,
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

// todo: union
typedef int TokenValue;

typedef struct Token {
  TokenType  type;
  char      *start;
  int        size;
  int        line;
  int        col;
  TokenValue value;
} Token;

// (name, literal)
typedef char *TokenTable[][2];

typedef struct Lexer {
  int        line;
  int        col;
  Buffer     input;
  Token      current;
  TokenTable token_table;
} Lexer;

Token lex_next_token(Lexer *lex);

Lexer *lex_new(char *input, int size);

bool lex_is_eof(Lexer *lex);
void lex_print_token(Lexer *lex, Token tok);

#endif // HOLYC_LEX
