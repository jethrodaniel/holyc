#ifndef HOLYC_SRC_CODEGEN
#define HOLYC_SRC_CODEGEN

#include "lib/stdio.c"

#include "src/cc.c"

void emit_mov_rax_imm(CC *cc, uint64_t imm) {
  if (cc->output_asm)
    return printf("MOV RAX,%d\n", imm);

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xB8; // MOV RAX,imm
  for (int i = 0; i < 8; i++)
    *cc->code++ = imm >> 8 * i;
}

void emit_sub_rax_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("SUB RAX,RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x29; // SUB RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_add_rax_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("ADD RAX,RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x01; // ADD RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_imul_rax_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("IMUL RAX,RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x0F; // IMUL RAX,reg
  *cc->code++ = 0xAF;
  *cc->code++ = 0xC7;
}

void emit_cqo_idiv_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("CQO\nIDIV RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x99; //  CQO

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xF7; //  IDIV
  *cc->code++ = 0xFF; //  RDI
}

void emit_push(CC *cc, int n) {
  if (cc->output_asm)
    return printf("PUSH %d\n", n);

  *cc->code++ = 0x68; // PUSH
  *cc->code = n;
  cc->code += 4;
}

void emit_push_rax(CC *cc) {
  if (cc->output_asm)
    return printf("PUSH RAX\n");

  *cc->code++ = 0x50; // PUSH RAX
}

void emit_pop_rax(CC *cc) {
  if (cc->output_asm)
    return printf("POP RAX\n");

  *cc->code++ = 0x58; // POP RAX
}

void emit_pop_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("POP RDI\n");

  *cc->code++ = 0x5F; // POP RDI
}

void emit_syscall(CC *cc) {
  if (cc->output_asm)
    return printf("  SYSCALL");

  *cc->code++ = 0x0F; // SYSCALL
  *cc->code++ = 0x05;
}

void emit_start(CC *cc) {
  if (cc->output_asm)
    return printf("\n_start:\n"
                  "  // CALL main\n"
                  "  MOV RDI,RAX  // arg1, main()\n"
                  "  MOV RAX,60   // exit\n"
                  "  SYSCALL      // exit()\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x89; // MOV RDI,reg
  *cc->code++ = 0xC7; //   RAX

  emit_mov_rax_imm(cc, SYSCALL_EXIT);
  emit_syscall(cc);
}

#endif // HOLYC_SRC_CODEGEN
