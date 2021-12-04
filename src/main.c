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

#include "lib/crt0.c"
#include "lib/mman.c"
#include "lib/stdbool.h"
#include "lib/stddef.h"
#include "lib/stdint.h"
#include "lib/stdio.c"
#include "lib/stdlib.c"
#include "lib/string.c"
#include "lib/unistd.c"

#include "src/elf.c"
#include "src/macho.c"

#include "src/cc.c"
#include "src/codegen.c"
#include "src/lex.c"
#include "src/parse.c"

#define INPUT_SIZE 4096

// Read code from stdin, options from argv, output asm or binary to
// stdout.
//
int main(int argc, char **argv, char **envp) {
  char input[INPUT_SIZE];
  int  num_read;

  CC *cc = malloc(sizeof(CC));
  cc->argc = argc;
  cc->argv = argv;
  cc->envp = envp;
  cc->input_buf = malloc(sizeof(char) * INPUT_SIZE);
  cc->input = cc->input_buf;
  cc->code_buf = malloc(sizeof(char) * INPUT_SIZE);
  cc->code = cc->code_buf;
  parse_options(cc);

  char *token_table[][2] = {
      {"EOF",    "\\0"},
      {"INT",    ""   },
      {"MIN",    "-"  },
      {"PLUS",   "+"  },
      {"DIV",    "/"  },
      {"MUL",    "*"  },
      {"LPAREN", "("  },
      {"RPAREN", ")"  },
  };

  for (int i = 0; i < sizeof(token_table); i++) {
    cc->token_table[i][0] = token_table[i][0];
    cc->token_table[i][1] = token_table[i][1];
  }

  if ((cc->input_size = read(STDIN_FILENO, cc->input, INPUT_SIZE)) < 0)
    die("read");

  warnf("read %d bytes\n", cc->input_size);

  _root(cc);

  if (cc->output_asm)
    return EXIT_SUCCESS;

  emit_pop_rax(cc);
  emit_start(cc);

  int code_size = cc->code - cc->code_buf;

  warnf("Writing %d bytes of machine code\n", code_size);

#ifdef __APPLE__
  write_macho(cc->code_buf, code_size);
#else
  write_elf(cc->code_buf, code_size);
#endif

  return EXIT_SUCCESS;
}
