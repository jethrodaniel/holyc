#ifndef HOLYC_SRC_LEX
#define HOLYC_SRC_LEX

#include "lib/stdio.c"

#include "src/cc.c"

typedef enum {
  TK_EOF,
  TK_INT,
  TK_MIN,
  TK_PLUS,
  TK_DIV,
  TK_MUL,
  TK_LPAREN,
  TK_RPAREN,
} TokenType;

// Print a token for debugging.
//
void print_token(CC *cc) {
  char *tokname = cc->token_table[cc->token][0];

  if (!tokname)
    error("%s: not sure how to print token %d\n", __func__, cc->token);

  warnf("[%s, ", tokname);

  if (cc->token == TK_INT)
    warnf("'%d'", cc->int_val);
  else
    warnf("'%s'", cc->token_table[cc->token][1]);

  warnf("]\n", tokname);
}

// Fetches next token.
//
int Lex(CC *cc) {
  char *c = cc->input;
  int   n;

  // if (c > cc->input_size) {
  //   cc->token = TK_EOF;
  //   warnf("too big: ");
  //   goto ret;
  // }

  // if (cc->token == TK_EOF)
  // goto ret;

  while (true) {
    switch (*c) {
    case '\n':
    case ' ':
      c++;
      break;
    case '\0':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_EOF;
      goto ret;
    case '+':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_PLUS;
      goto ret;
    case '-':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_MIN;
      goto ret;
    case '*':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_MUL;
      goto ret;
    case '/':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_DIV;
      goto ret;
    case '(':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_LPAREN;
      goto ret;
    case ')':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_RPAREN;
      goto ret;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      cc->token_pos = c;

      n = 0;

      do {
        n = n * 10 + *c - '0';
        c++;
      } while (*c >= '0' && *c <= '9');

      cc->int_val = n;
      cc->input = c;
      cc->token = TK_INT;
      goto ret;
    default:
      error("unexpected character '%c' (%d) at column %d\n", *c, *c,
            cc->input - cc->input_buf);
    }
  }

ret:
  print_token(cc);
  return cc->token;
}

// Unfetches next token. HACKY
//
void Unlex(CC *cc) {
  cc->input = cc->token_pos;
}


#endif // HOLYC_SRC_LEX
