#include <holyc/ast.h>
#include <holyc/parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse(char *input) {
  printf("'%s'\n", input);

  Parser *parser = malloc(sizeof(Parser));
  parse_new(parser, input, strlen(input));

  AstNode *node = parse_parse(parser);

  parse_print_node(parser, node);

  printf("\n");
}

int main(int argc, char **argv) {
  printf("== parser tests ===\n\n");

  parse("42;");
  parse("1 + 2;");
  parse("1 - 2;");
  parse("1 + 2 - 3;");
  parse(";");

  return 0;
}
