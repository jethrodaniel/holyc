#include <holyc/lex.h>

#define LEX_NUM_TOKENS 9

// {"NAME", "LITERAL"}
typedef char *TokenTable[LEX_NUM_TOKENS][2];

typedef struct Lexer {
  int         line;
  int         col;
  Buffer      input;
  Token       current;
  TokenTable *token_table;
} Lexer;

Token lex_next_token(Lexer *lex) {
  return lex->current;
}

void lex_print_token(Lexer *lex, Token tok) {
  char *tokname = lex->token_table[tok.type][0][0];

  if (!tokname)
    error("%s: not sure how to print token %d\n", __func__, tok.type);

  warnf("[%d:%d][%s, ", tok.line + 1, tok.col + 1, tokname);

  if (tok.type == TK_INT)
    warnf("'%d'", tok.value);
  else
    warnf("'%s'", lex->token_table[tok.type][1]);

  warnf("]\n", tokname);
}

//

void print_token(CC *cc) {
  Token t = cc->parser.current;

  char *tokname = cc->token_table[t.type][0];

  if (!tokname)
    error("%s: not sure how to print token %d\n", __func__, t.type);

  warnf("[%d:%d][%s, ", t.line + 1, t.col + 1, tokname);

  if (t.type == TK_INT)
    warnf("'%d'", t.value);
  else
    warnf("'%s'", cc->token_table[t.type][1]);

  warnf("]\n", tokname);
}

void lexer_init(CC *cc) {
  Lex(cc);
  cc->parser.previous = cc->parser.current;
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
      c++;
      cc->parser.line++;
      cc->parser.col = 0;
      break;
    case ' ':
      c++;
      cc->parser.line++;
      break;
    case '\0':
      cc->parser.current.start = c;
      cc->input.curr = c;
      cc->input.curr++;
      cc->parser.current.type = TK_EOF;
      goto ret;
    case ';':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_SEMI;
      goto ret;
    case '+':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_PLUS;
      goto ret;
    case '-':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_MIN;
      goto ret;
    case '*':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_MUL;
      goto ret;
    case '/':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_DIV;
      goto ret;
    case '(':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_LPAREN;
      goto ret;
    case ')':
      cc->parser.current.start = c;
      cc->input.curr = ++c;
      cc->parser.current.type = TK_RPAREN;
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
      cc->parser.current.start = c;

      n = 0;

      do {
        n = n * 10 + *c - '0';
        c++;
      } while (*c >= '0' && *c <= '9');

      cc->parser.current.value = n;
      cc->input.curr = c;
      cc->parser.current.type = TK_INT;
      goto ret;
    default:
      error("-- error: unexpected character '%c' (%d) at column %d\n", *c, *c,
            cc->input.curr - cc->input.start);
    }
  }

ret:
  cc->parser.current.size = c - cc->parser.current.start;
  cc->parser.col += cc->parser.current.size;

  cc->parser.current.line = cc->parser.line;
  cc->parser.current.col = cc->parser.col - cc->parser.current.size;

  if (cc->opts->debug & DEBUG_LEX)
    print_token(cc);

  return cc->parser.current.type;
}

// Unfetches next token. HACKY
//
void Unlex(CC *cc) {
  if (cc->opts->debug & DEBUG_LEX)
    warnf("[lexer] Unlex()\n");
  cc->input.curr = cc->parser.current.start;
}
