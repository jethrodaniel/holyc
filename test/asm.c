#include <holyc/ast.h>
#include <holyc/parse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void _print_asm(AstNode *node, int indent) {
  switch (node->type) {
  case NODE_UNINITIALIZED:
    break;
  case NODE_INT:
    printf("  PUSH %d\n", node->value);
    break;
  case NODE_BINOP_PLUS:
  case NODE_BINOP_MIN:
  case NODE_BINOP_MUL:
  case NODE_BINOP_DIV:
    printf("  POP RDI\n");
    printf("  POP RDX\n");

    if (node->type == NODE_BINOP_MUL)
      printf("  IMUL RAX,RDI\n");
    else
      printf("  CQO IDIV RDI\n");

    printf("  PUSH RAX\n");
    // _parse_print_node(parser, node->left, indent + 2);
    // _parse_print_node(parser, node->right, indent + 2);
    break;
  default: {
  }
  }
}

void print_asm(AstNode *node) {
  printf("\nmain:\n");

  _print_asm(node, 0);

  printf("  POP RAX\n");

  printf("\n_start:\n");
  printf("  // CALL main\n");
  printf("  MOV RDI, RDX\n");
  printf("  MOV RAX, %d\n", SYSCALL_EXIT);
  printf("  SYSCALL\n");
}

void test_asm(char *input) {
  printf("'%s'\n", input);

  Parser *parser = malloc(sizeof(Parser));
  parse_new(parser, input, strlen(input));

  AstNode *node = parse_parse(parser);

  print_asm(node);

  printf("\n");
}

int main(int argc, char **argv) {
  printf("== asm tests ===\n\n");

  test_asm("42;");
  test_asm("1 + 2;");
  test_asm("1 - 2;");
  test_asm("1 + 2 - 3;");
  test_asm("3 * 4;");
  test_asm("3 / 4;");
  test_asm("3 / 4 * 5;");
  test_asm("1 + 2 * 3;");
  test_asm("(1 + 2) * 3;");
  test_asm("(1+3-1)*(2*3/2)-7;");

  return 0;
}
