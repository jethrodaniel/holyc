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

#include <lib/c/crt0.c>
#include <lib/c/unistd.c>

#include "src/cc.c"
#include "src/codegen.c"
#include "src/lex.c"
#include "src/obj.c"
#include "src/parse.c"

#define INPUT_SIZE 4096

// Read code from stdin, options from argv, output asm or binary to
// stdout.
//
int main(int argc, char **argv, char **envp) {
  CC *cc = cc_init(argc, argv, envp, INPUT_SIZE);

  if ((cc->input_size = read(STDIN_FILENO, cc->input, INPUT_SIZE)) < 0)
    die("read");

  // warnf("read %d bytes\n", cc->input_size);

  _root(cc);
  emit_start(cc);

  if (cc->opts->output_asm)
    return EXIT_SUCCESS;

  write_executable(cc);

  return EXIT_SUCCESS;
}
