// = holyc
//
// SysV x86_64 fn call registers (in order), rest on the stack
//
// - rdi
// - rsi
// - rdx
// - rcx
// - r8
// - r9
//
// Syscalls use:
//
//  rdi
//  rsi
//  rdx
//  r10
//  r9
//
//  and clobbers rcx, r11, and rax

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <holyc/cc.h>
#include <holyc/codegen.h>
#include <holyc/lex.h>
#include <holyc/obj.h>
#include <holyc/parse.h>

#define INPUT_SIZE 4096

int main(int argc, char **argv, char **envp) {
  CC *cc = cc_init(argc, argv, envp, INPUT_SIZE);

  if (cc_read(cc, STDIN_FILENO, INPUT_SIZE) < 0)
    die("cc_read");

  _root(cc);
  emit_start(cc);

  if (cc->opts->output_asm)
    return EXIT_SUCCESS;

  write_executable(cc);

  return EXIT_SUCCESS;
}
