#ifndef HOLYC_SRC_CC
#define HOLYC_SRC_CC

#include "lib/stdbool.h"
#include "lib/stdio.c"
#include "lib/stdlib.c"

// Global compiler state.
//
typedef struct CC {
  // globals from main()
  int    argc;
  char **argv;
  char **envp;
  bool   output_asm;       // output asm, or binary?
  char  *input_buf;        // input source buffer
  char  *input;            // curr position in source buffer
  int    input_size;       // length of input
  char  *code_buf;         // output code buffer
  char  *code;             // curr position in code buffer
  char  *token_pos;        // token start index
  int    token;            // token type
  int    int_val;          // if token is int
  char  *token_table[][2]; // token names
} CC;

// Set defaults, update cc options from argv values.
//
void parse_options(CC *cc) {
  for (int i = 0; i < cc->argc; i++) {
    char *arg = cc->argv[i];

    if (*arg == '-')
      arg++;
    else
      continue;

    if (*arg == 'S')
      cc->output_asm = true;
  }
}

void debug(CC *cc) {
  warnf("\n=== cc ===\n");
  warnf("\tinput_buf: %s (%d)\n", cc->input_buf, cc->input_buf);
  warnf("\tinput_size: %d\n", cc->input_size);
  warnf("\tcode: %d\n", cc->code);
  warnf("\ttoken: %d | ", cc->token);
  warnf("==========\n");
}

// Print to stderr and exit.
//
void error(char *fmt, ...) {
  __asm__("call _warnf");
  exit(1);
}

#endif // HOLYC_SRC_CC
