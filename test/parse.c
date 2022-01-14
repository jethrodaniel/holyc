#include <holyc/parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse(char *input) {
  printf("'%s'\n", input);

  Parser *parser = parse_new(input, strlen(input));

  // Token tok;

  // while (!lex_is_eof(lex)) {
  //   tok = lex_next_token(lex);
  //   lex_print_token(lex, tok);
  // }

  printf("\n");
}

int main(int argc, char **argv) {
  printf("== parser tests ===\n\n");

  parse("42;");

  return 0;
}
