#include <holyc/ast.h>
#include <holyc/compile.h>
#include <holyc/parse.h>

#include <stdio.h>
#include <stdlib.h>

void compile(char *input) {
  printf("'%s'\n", input);

  Parser *parser = malloc(sizeof(Parser));
  parse_new(parser, input, strlen(input));

  AstNode *node = parse_parse(parser);

  // Compiler *cc = malloc(sizeof(Compiler));
  // compile_new(cc);
  compiler_compile(node);

  printf("\n");
}

int main(int argc, char **argv) {
  printf("== compiler tests ===\n\n");

  compile("42;");
  compile("1 + 2;");
  compile("1 - 2;");
  compile("1 + 2 - 3;");
  // compile("3 * 4;");
  // compile("3 / 4;");
  // compile("3 / 4 * 5;");
  // compile("1 + 2 * 3;");
  // compile("(1 + 2) * 3;");
  // compile("(1+3-1)*(2*3/2)-7;");
  // compile(";");

  return 0;
}
