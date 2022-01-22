#include <holyc/ast.h>
#include <holyc/compile.h>

#include <stdio.h>
#include <stdlib.h>

//====

// PUSH imm
//
static void emit_push(int imm) {
  printf("PUSH %d\n", imm);
  // *cc->code.curr++ = 0x68; // PUSH
  // *cc->code.curr   = imm;
  // cc->code.curr += 4;
}

// POP RAX
//
static void emit_pop_rax() {
  printf("POP RAX\n");
  // *cc->code.curr++ = 0x58; // POP RAX
}

// main:
//
static void emit_main_label() {
  printf("main:\n");
}

//----

// Compiler *compile_new(Compiler *cc) {
//   return cc;
// }

static void compiler_visit_int(AstNode *node) {
  printf("int: %d\n", node->value);
  return emit_push(node->value);
}

static void compiler_visit_binop(AstNode *node) {
  printf("binop\n");

  switch (node->type) {
  case NODE_BINOP_PLUS:
  case NODE_BINOP_MIN:
  case NODE_BINOP_MUL:
  case NODE_BINOP_DIV:
  default:
    warnf("%s: unknown node type %d\n", __func__, node->type);
    exit(1);
  }
}

static void compiler_visit_expr(AstNode *node) {
  printf("expr\n");

  switch (node->expr_value->type) {
  case NODE_INT:
    return compiler_visit_int(node->expr_value);
  case NODE_BINOP_PLUS:
  case NODE_BINOP_MIN:
  case NODE_BINOP_MUL:
  case NODE_BINOP_DIV:
    return compiler_visit_binop(node->expr_value);
  default:
    warnf("%s: unknown node type %d\n", __func__, node->type);
    exit(1);
  }
}

static void compiler_visit(AstNode *node) {
  switch (node->type) {
  case NODE_EXPR: // root
    emit_main_label();
    compiler_visit_expr(node);
    return emit_pop_rax();
  case NODE_INT:
    return compiler_visit_int(node);
  default:
    warnf("unknown node type %d\n", node->type);
    exit(1);
  }
}

Compilation *compiler_compile(AstNode *node) {
  Compilation *cc = malloc(sizeof(Compilation));

  compiler_visit(node);

  printf("COMPILE\n");
  return cc;
}
