#ifndef HOLYC_CC
#define HOLYC_CC

#include <holyc/buffer.h>

#include <stdbool.h>

enum DebugFlags {
  DEBUG_LEX   = (1 << 0),
  DEBUG_PARSE = (1 << 1),
  DEBUG_OBJ   = (1 << 2),
};

typedef struct MainArgs {
  int    argc;
  char **argv;
  char **envp;
} MainArgs;

typedef int Flag;

typedef struct CompilerOpts {
  bool output_asm;
  Flag debug;
} CompilerOpts;

#include <holyc/_parse.h>

// Global compiler state.
//
typedef struct CC {
  CompilerOpts *opts;
  MainArgs      main_args;
  Buffer        code;
  Parser        parser;
} CC;

void error(char *fmt, ...);

CC *cc_init(int argc, char **argv, char **envp, int input_size);
int cc_read(CC *cc, int fd, int size);

#endif // HOLYC_CC
