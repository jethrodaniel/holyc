#ifndef HOLYC_CC
#define HOLYC_CC

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct CompilerOpts {
  bool output_asm;   // output asm, or binary?
  bool debug_obj;    // print verbose object file info
  bool debug_parser; // print verbose parser info
} CompilerOpts;

// Global compiler state.
//
typedef struct CC {
  // globals from main()
  int           argc;
  char        **argv;
  char        **envp;
  char         *input_buf;        // input source buffer
  char         *input;            // curr position in source buffer
  int           input_size;       // length of input
  char         *code_buf;         // output code buffer
  char         *code;             // curr position in code buffer
  char         *token_pos;        // token start index
  int           token;            // token type
  int           int_val;          // if token is int
  CompilerOpts *opts;             // compiler options
  char         *token_table[][2]; // token names
} CC;

void debug(CC *cc);

void error(char *fmt, ...);

CC *cc_init(int argc, char **argv, char **envp, int input_size);

#endif // HOLYC_CC