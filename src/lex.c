#include <holyc/lex.h>

void print_token(CC *cc) {
  char *tokname = cc->token_table[cc->curr_token.type][0];

  if (!tokname)
    error("%s: not sure how to print token %d\n", __func__, cc->curr_token.type);

  warnf("[%s, ", tokname);

  if (cc->curr_token.type == TK_INT)
    warnf("'%d'", cc->int_val);
  else
    warnf("'%s'", cc->token_table[cc->curr_token.type][1]);

  warnf("]\n", tokname);
}

// Fetches next token.
//
int Lex(CC *cc) {
  if (cc->opts->debug & DEBUG_LEX)
    warnf("[lexer] Lex(): ");

  char *c = cc->input.curr;
  int   n;

  while (true) {
    switch (*c) {
    case '\n':
    case ' ':
      c++;
      break;
    case '\0':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_EOF;
      goto ret;
    case ';':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_SEMI;
      goto ret;
    case '+':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_PLUS;
      goto ret;
    case '-':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_MIN;
      goto ret;
    case '*':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_MUL;
      goto ret;
    case '/':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_DIV;
      goto ret;
    case '(':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_LPAREN;
      goto ret;
    case ')':
      cc->curr_token.start = c;
      cc->input.curr = ++c;
      cc->curr_token.type = TK_RPAREN;
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
      cc->curr_token.start = c;

      n = 0;

      do {
        n = n * 10 + *c - '0';
        c++;
      } while (*c >= '0' && *c <= '9');

      cc->int_val = n;
      cc->input.curr = c;
      cc->curr_token.type = TK_INT;
      goto ret;
    default:
      error("-- error: unexpected character '%c' (%d) at column %d\n", *c, *c,
            cc->input.curr - cc->input.start);
    }
  }

ret:
  if (cc->opts->debug & DEBUG_LEX)
    print_token(cc);
  return cc->curr_token.type;
}

// Unfetches next token. HACKY
//
void Unlex(CC *cc) {
  if (cc->opts->debug & DEBUG_LEX)
    warnf("[lexer] Unlex()\n");
  cc->input.curr = cc->curr_token.start;
}
