#ifndef HOLYC_SRC_COMPILE
#define HOLYC_SRC_COMPILE

#include <holyc/ast.h>
#include <holyc/buffer.h>

typedef enum CompilationType {
  TOP_LEVEL,
  FUNCTION,
} CompilationType;

typedef struct Compilation {
  CompilationType type;
  Buffer          code;
} Compilation;

typedef struct Compiler {
} Compiler;

// void compiler_visit_expr(AstNode *node);
// void compiler_visit_int(AstNode *node);
// void compiler_visit(AstNode *node);

// Compiler *compile_new(Compiler *cc);
Compilation *compiler_compile(AstNode *node);

#endif // HOLYC_SRC_COMPILE
