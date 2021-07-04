// = holyc
//
// == references
//
// - Terry Davis's mini compiler demo:
//   https://github.com/cia-foundation/TempleOS/blob/archive/Demo/Lectures/MiniCompiler.HC
//
// - Michael Lazear's assembler:
//   https://github.com/lazear/lass/blob/master/assembler.c#L53
//
// - Elf quine: https://medium.com/@MrJamesFisher/understanding-the-elf-4bd60daac571
//   https://medium.com/@MrJamesFisher/understanding-the-elf-4bd60daac571

// SysV x86_64 fn call registers (in order), rest on the stack
//
// - rdi
// - rsi
// - rdx
// - rcx
// - r8
// - r9

// Linux syscalls use:
//
//  rdi
//  rsi
//  rdx
//  r10
//  r9
//
//  clobbers %rcx, %r11, and %rax


#include "src/boot.c"
#include "src/stddef.h"
#include "src/stdint.h"
#include "src/mman.c"
#include "src/stdlib.c"
#include "src/unistd.c"
#include "src/string.c"
#include "src/stdio.c"

#include "src/elf.c"

#define INPUT_SIZE 4096

int main(int argc, char **argv, char **envp) {
  char input[INPUT_SIZE];
  int num_read;

  if ((num_read = read(STDIN_FILENO, input, INPUT_SIZE)) < 0)
    die("read");

  warnf("read %d bytes\n", num_read);

  write_elf_header(num_read);
  write(STDOUT_FILENO, input, num_read - 1); // rm \n

  return EXIT_SUCCESS;
}
