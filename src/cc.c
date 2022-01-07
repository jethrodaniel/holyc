#include <stdio.h>
#include <stdlib.h>

#include <holyc/cc.h>

void debug(CC *cc) {
  warnf("\n=== cc ===\n");
  warnf("\tinput_buf: %s (%d)\n", cc->input.start, cc->input.start);
  warnf("\tinput_size: %d\n", cc->input.size);
  warnf("\tcode: %d\n", cc->code.curr);
  warnf("\ttoken: %d | ", cc->token);
  warnf("==========\n");
}

// Print to stderr and exit.
//
void error(char *fmt, ...) {
  __asm__("call _warnf");
  exit(1);
}

static void parse_options(CC *cc) {
  for (int i = 0; i < cc->main_args.argc; i++) {
    char *arg = cc->main_args.argv[i];

    if (*arg != '-')
      continue;

    if (strcmp(arg, "-S") == 0)
      cc->opts->output_asm = true;
    else if (strcmp(arg, "--debug-obj") == 0)
      cc->opts->debug |= DEBUG_OBJ;
    else if (strcmp(arg, "--debug-parser") == 0)
      cc->opts->debug |= DEBUG_PARSE;
    else if (strcmp(arg, "--debug-lexer") == 0)
      cc->opts->debug |= DEBUG_LEX;
    else {
      warnf("unknown option '%s'\n", arg);
      exit(1);
    }
  }
}

CC *cc_init(int argc, char **argv, char **envp, int input_size) {
  CC *cc = malloc(sizeof(CC));
  cc->main_args.argc = argc;
  cc->main_args.argv = argv;
  cc->main_args.envp = envp;
  cc->input.start = malloc(sizeof(char) * input_size);
  cc->input.curr = cc->input.start;
  cc->code.start = malloc(sizeof(char) * input_size);
  cc->code.curr = cc->code.start;

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
