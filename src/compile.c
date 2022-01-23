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

// PUSH RAX
//
static void emit_push_rax() {
  printf("PUSH RAX\n");
  // *cc->code.curr++ = 0x50; // PUSH RAX
}

// POP RAX
//
static void emit_pop_rax() {
  printf("POP RAX\n");
  // *cc->code.curr++ = 0x58; // POP RAX
}

// POP RDI
//
static void emit_pop_rdi() {
  printf("POP RDI\n");
  // *cc->code.curr++ = 0x5F; // POP RDI
}

static void emit_add_rax_rdi() {
  printf("ADD RAX, RDI\n");

  // *cc->code.curr++ = 0x48; // REX
  // *cc->code.curr++ = 0x01; // ADD RAX,reg
  // *cc->code.curr++ = 0xF8; //  RDI
}

static void emit_sub_rax_rdi() {
  printf("SUB RAX, RDI\n");

  // *cc->code.curr++ = 0x48; // REX
  // *cc->code.curr++ = 0x29; // SUB RAX,reg
  // *cc->code.curr++ = 0xF8; //  RDI
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

static void compiler_visit(AstNode *node);

static void compiler_visit_int(AstNode *node) {
  // printf("int: %d\n", node->value);
  return emit_push(node->value);
}

static void compiler_visit_binop(AstNode *node) {
  // printf("binop\n");

  compiler_visit(node->left);
  compiler_visit(node->right);
  emit_pop_rdi();
  emit_pop_rax();

  // return compiler_visit(node->expr_value);
  switch (node->type) {
  case NODE_BINOP_PLUS:
    emit_add_rax_rdi();
    break;
  case NODE_BINOP_MIN:
    emit_sub_rax_rdi();
    break;
  case NODE_BINOP_MUL:
  case NODE_BINOP_DIV:
  default:
    warnf("%s: unknown node type %d\n", __func__, node->type);
    exit(1);
  }

  emit_push_rax();
}

static void compiler_visit_expr(AstNode *node) {
  // printf("expr\n");
  return compiler_visit(node->expr_value);
}

static void compiler_visit(AstNode *node) {
  switch (node->type) {
  case NODE_EXPR: // root
    emit_main_label();
    compiler_visit_expr(node);
    return emit_pop_rax();
  case NODE_INT:
    return compiler_visit_int(node);
  case NODE_BINOP_PLUS:
  case NODE_BINOP_MIN:
  case NODE_BINOP_MUL:
  case NODE_BINOP_DIV:
    return compiler_visit_binop(node);
  default:
    warnf("unknown node type %d\n", node->type);
    exit(1);
  }
}

Compilation *compiler_compile(AstNode *node) {
  Compilation *code = malloc(sizeof(Compilation));

  compiler_visit(node);

  return code;
}
