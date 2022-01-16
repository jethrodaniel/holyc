#include <holyc/ast.h>

AstNode *ast_new(AstNode *node, AstType type, int value) {
  node->type  = type;
  node->value = value;
  return node;
}
