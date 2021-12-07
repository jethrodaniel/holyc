#ifndef HOLYC_SRC_CODEGEN
#define HOLYC_SRC_CODEGEN

#include "lib/stdio.c"

#include "src/cc.c"
#include "src/elf.c"
#include "src/macho.c"

void emit_mov_rax_imm(CC *cc, uint64_t imm) {
  if (cc->opts->output_asm)
    return printf("\tmov rax,%d\n", imm);

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xB8; // MOV RAX,imm
  for (int i = 0; i < 8; i++)
    *cc->code++ = imm >> 8 * i;
}

void emit_sub_rax_rdi(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tsub rax,rdi\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x29; // SUB RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_add_rax_rdi(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tadd rax,rdi\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x01; // ADD RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_imul_rax_rdi(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\timul rax,rdi\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x0F; // IMUL RAX,reg
  *cc->code++ = 0xAF;
  *cc->code++ = 0xC7;
}

void emit_cqo_idiv_rdi(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tcqo\nidiv rdi\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x99; //  CQO

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xF7; //  IDIV
  *cc->code++ = 0xFF; //  RDI
}

void emit_push(CC *cc, int n) {
  if (cc->opts->output_asm)
    return printf("\tpush %d\n", n);

  *cc->code++ = 0x68; // PUSH
  *cc->code = n;
  cc->code += 4;
}

void emit_mov_rdi_rax(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tmov rdi,rax\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x89; // MOV RDI,reg
  *cc->code++ = 0xC7; //   RAX
}

void emit_push_rax(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tpush rax\n");

  *cc->code++ = 0x50; // PUSH RAX
}

void emit_pop_rax(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tpop rax\n");

  *cc->code++ = 0x58; // POP RAX
}

void emit_pop_rdi(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tpop rdi\n");

  *cc->code++ = 0x5F; // POP RDI
}

void emit_syscall(CC *cc) {
  if (cc->opts->output_asm)
    return printf("\tsyscall\n");

  *cc->code++ = 0x0F; // SYSCALL
  *cc->code++ = 0x05;
}

void emit_start(CC *cc) {
  if (cc->opts->output_asm)
    printf("\n_start:\n\t// call main\n");

  emit_mov_rdi_rax(cc);
  emit_mov_rax_imm(cc, SYSCALL_EXIT);
  emit_syscall(cc);
}

void emit_main_label(CC *cc) {
  if (cc->opts->output_asm)
    printf("\nmain:\n");
}

void write_executable(CC *cc) {
  int code_size = cc->code - cc->code_buf;
  warnf("Writing %d bytes of machine code\n", code_size);
#ifdef __APPLE__
  write_macho(cc->code_buf, code_size);
#else
  write_elf(cc->code_buf, code_size);
#endif
}

#endif // HOLYC_SRC_CODEGEN
