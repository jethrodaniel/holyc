#include <holyc/ast.h>

AstNode *ast_new(AstNode *node, int value) {
  node->type  = NODE_INT;
  node->value = value;
  return node;
}
