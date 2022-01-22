#ifndef HOLYC_AST
#define HOLYC_AST

typedef enum {
  NODE_UNINITIALIZED,
  NODE_INT,
  NODE_BINOP_PLUS,
  NODE_BINOP_MIN,
  NODE_BINOP_MUL,
  NODE_BINOP_DIV,
  NODE_EXPR,
} AstType;

#define NUM_NODES 7

typedef char *NodeNameTable[NUM_NODES];

typedef struct AstNode {
  AstType type;
  int     value;

  struct AstNode *left;
  struct AstNode *right;

  struct AstNode *expr_value;
} AstNode;

// typedef struct BinOpNode {
//   AstNode   left;
//   AstNode   right;
//   BinOpType type;
// } BinOpNode;

AstNode *ast_new(AstNode *node, AstType type, int value);

#endif // HOLYC_AST
