#include <holyc/lex.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lex(char *input) {
  printf("'%s'\n", input);

  Lexer *lex = malloc(sizeof(Lexer));
  lex_new(lex, input, strlen(input));

  Token tok;

  while (!lex_is_eof(lex)) {
    tok = lex_next_token(lex);
    lex_print_token(lex, tok);
  }

  printf("\n");
}

int main(int argc, char **argv) {
  printf("== lexer tests ===\n\n");

  lex("");
  lex(";");
  lex("+");
  lex("-");
  lex("*");
  lex("/");
  lex("(");
  lex(")");
  lex("   ");
  lex(" \n ");
  lex("1");
  lex("123\n45;");

  return 0;
}
