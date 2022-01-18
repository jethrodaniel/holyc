#ifndef HOLYC_CC
#define HOLYC_CC

#include <holyc/buffer.h>

#include <stdbool.h>

typedef struct MainArgs {
  int    argc;
  char **argv;
  char **envp;
} MainArgs;

#include <holyc/_parse.h>

// Global compiler state.
//
typedef struct CC {
  MainArgs main_args;
  Buffer   code;
  Parser   parser;
} CC;

void error(char *fmt, ...);

CC *cc_init(int argc, char **argv, char **envp, int input_size);
int cc_read(CC *cc, int fd, int size);

#endif // HOLYC_CC
