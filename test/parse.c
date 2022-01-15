#include <holyc/ast.h>
#include <holyc/parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse(char *input) {
  printf("'%s'\n", input);

  Parser *parser = malloc(sizeof(Parser));
  parse_new(parser, input, strlen(input));

  AstNode *node = malloc(sizeof(AstNode));
  parse_parse(parser, node);
  parse_print_node(parser, node);

  printf("\n");
}

int main(int argc, char **argv) {
  printf("== parser tests ===\n\n");

  parse("42;");

  return 0;
}
