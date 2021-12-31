#include "test.c"
#include <holyc/cc.h>
#include <holyc/lex.h>

typedef struct Token {
  int       line;
  int       col;
  TokenType type;
  char     *text;
} Token;

typedef struct Lexer {
  char  *input; // input source code
  int    line;
  int    col;
  Token *tokens;
} Lexer;

Token *hc_token_new() {
  Token *t = malloc(sizeof(Token));
  t->line = 1;
  t->col = 1;
  return t;
}

void test_token(Test *t) {
  Token *tok = hc_token_new();
  assert(t, tok->line == 1, "initial token line is 1");
  assert(t, tok->col == 1, "initial token column is 1");
}

Lexer *hc_lexer_new() {
  Lexer *lex = malloc(sizeof(Lexer));
  (void)lex;
  return lex;
}

void test_lexer(Test *t) {
  // Lexer *lex = hc_lexer_new();
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
      {test_token,    "test_token"   },
      {test_lexer,    "test_lexer"   },
      {test_cc_setup, "test_cc_setup"},
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
