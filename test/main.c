#include "test.c"
#include <holyc/cc.h>
#include <holyc/lex.h>

typedef struct Token {
  int       line;
  int       col;
  TokenType type;
  union {
    int int_v;
  } value;
} Token;

typedef struct Lexer {
  char  *input;     // current location in the input
  char  *input_buf; // input source code
  int    line;
  int    col;
  Token *tokens;
  int    num_tokens;
} Lexer;

Token *hc_token_new(TokenType type) {
  Token *t = malloc(sizeof(Token));
  t->line = 1;
  t->col = 1;
  t->type = type;
  return t;
}

void test_token(Test *t) {
  Token *tok = hc_token_new(TK_EOF);
  assert(t, tok->line == 1, "initial token line is 1");
  assert(t, tok->col == 1, "initial token column is 1");
}

Lexer *hc_lexer_new(char *input) {
  Lexer *lex = malloc(sizeof(Lexer));
  lex->line = 1;
  lex->col = 1;
  lex->input_buf = input;
  lex->input = input;
  return lex;
}

char hc_lexer_next_char(Lexer *lex) {
  char c = *lex->input++;

  if (c == '\n') {
    lex->line++;
    lex->col = 1;
  } else {
    lex->col++;
  }

  return c;
}

Token *hc_lexer_next(Lexer *lex) {
  char  *c;
  int    n;
  Token *tok = malloc(sizeof(Token));

  while (true) {
    c = lex->input;

    switch (*c) {
    case '\n':
    case ' ':
      hc_lexer_next_char(lex);
      goto ret;
    case '\0':
      tok = hc_token_new(TK_EOF);
      goto ret;
    case ';':
      tok = hc_token_new(TK_SEMI);
      goto ret;
    case '+':
      tok = hc_token_new(TK_PLUS);
      goto ret;
    case '-':
      tok = hc_token_new(TK_MIN);
      goto ret;
    case '*':
      tok = hc_token_new(TK_MUL);
      goto ret;
    case '/':
      tok = hc_token_new(TK_DIV);
      goto ret;
    case '(':
      tok = hc_token_new(TK_LPAREN);
      goto ret;
    case ')':
      tok = hc_token_new(TK_RPAREN);
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
      n = 0;

      do {
        n = n * 10 + *c - '0';
        c++;
      } while (*c >= '0' && *c <= '9');

      tok = hc_token_new(TK_INT);
      tok->value.int_v = n;
      goto ret;
    default:
      printf("-- error: unexpected character '%c' (%d) at column %d\n", *c, *c,
             lex->col);
    }
  }

ret:
  // print_token(cc);
  return tok;
}

void test_hc_lexer_new(Test *t) {
  Lexer *lex = hc_lexer_new("42;");
  assert(t, lex->line == 1, "initial lexer line is 1");
  assert(t, lex->col == 1, "initial lexer column is 1");
  assert(t, lex->num_tokens == 0, "initial lexer # of tokens is 0");
  assert(t, strcmp(lex->input, "42;") == 0, "lexer->text");
}

void test_hc_lexer_next_char(Test *t) {
  Lexer *lex = hc_lexer_new("42;");

  assert(t, lex->col == 1, "column is 1");
  assert(t, hc_lexer_next_char(lex) == '4', "\t4");
  assert(t, lex->col == 2, "column is 2");
  assert(t, hc_lexer_next_char(lex) == '2', "\t2");
}

#define IT_LEXS(input, expected_type)                                          \
  {                                                                            \
    lex = hc_lexer_new(input);                                                 \
    tok = hc_lexer_next(lex);                                                  \
    assert(t, tok->type == expected_type, input);                              \
  }

void test_hc_lexer_next(Test *t) {
  Lexer *lex;
  Token *tok;

  IT_LEXS("42;", TK_INT)
  IT_LEXS(";", TK_SEMI);
}

void test_cc_setup(Test *t) {
  char *argv[] = {
      "progname", //
      "arg1"      //
  };
  CC *cc = cc_init(2, argv, argv, 1024);

  assert(t, cc->token == 0, "initial token should be EOF");
}

void test_fail(Test *t) {
  t->passed = false;
}

int main(int argc, char **argv) {
  Test tests[] = {
      {test_token,              "test_token"                  },
      {test_hc_lexer_new,       "test_hc_lexer_new"           },
      {test_hc_lexer_next_char, "test_test_hc_lexer_next_char"},
      {test_hc_lexer_next,      "test_test_hc_lexer_next"     },
      {test_cc_setup,           "test_cc_setup"               },
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
