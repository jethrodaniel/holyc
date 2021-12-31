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
  char  *input;     // current location in the input
  char  *input_buf; // input source code
  int    line;
  int    col;
  Token *tokens;
  int    num_tokens;
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
      {test_cc_setup,           "test_cc_setup"               },
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
