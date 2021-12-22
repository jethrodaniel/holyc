#ifndef HOLYC_SRC_CODEGEN
#define HOLYC_SRC_CODEGEN

#include <holyc/cc.h>

#include <stdio.h>

void emit_mov_rax_imm(CC *cc, uint64_t imm);
void emit_sub_rax_rdi(CC *cc);
void emit_add_rax_rdi(CC *cc);

void emit_imul_rax_rdi(CC *cc);

void emit_cqo_idiv_rdi(CC *cc);

void emit_push(CC *cc, int n);

void emit_mov_rdi_rax(CC *cc);

void emit_push_rax(CC *cc);

void emit_pop_rax(CC *cc);

void emit_pop_rdi(CC *cc);

void emit_syscall(CC *cc);

void emit_start(CC *cc);

void emit_main_label(CC *cc);

#endif // HOLYC_SRC_CODEGEN
