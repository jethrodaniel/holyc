#ifndef HOLYC_SRC_CC
#define HOLYC_SRC_CC

#include "lib/stdbool.h"
#include "lib/stdio.c"
#include "lib/stdlib.c"

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

// Set defaults, updates cc options from argv values.
//
static void parse_options(CC *cc) {
  for (int i = 0; i < cc->argc; i++) {
    char *arg = cc->argv[i];

    if (*arg != '-')
      continue;

    if (strcmp(arg, "-S") == 0)
      cc->opts->output_asm = true;
    else if (strcmp(arg, "--debug-obj") == 0)
      cc->opts->debug_obj = true;
    else if (strcmp(arg, "--debug-parser") == 0)
      cc->opts->debug_parser = true;
    else {
      warnf("unknown option '%s'\n", arg);
      exit(1);
    }
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

CC *cc_init(int argc, char **argv, char **envp, int input_size) {
  CC *cc = malloc(sizeof(CC));
  cc->argc = argc;
  cc->argv = argv;
  cc->envp = envp;
  cc->input_buf = malloc(sizeof(char) * input_size);
  cc->input = cc->input_buf;
  cc->code_buf = malloc(sizeof(char) * input_size);
  cc->code = cc->code_buf;

  CompilerOpts *opts = malloc(sizeof(CompilerOpts));
  cc->opts = opts;

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
      {"SEMI",   ";"  },
  };

  for (int i = 0; i < (sizeof(token_table) / 16); i++) {
    cc->token_table[i][0] = token_table[i][0];
    cc->token_table[i][1] = token_table[i][1];
  }

  return cc;
}

#endif // HOLYC_SRC_CC
