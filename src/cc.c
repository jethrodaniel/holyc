#include <stdio.h>
#include <stdlib.h>

#include <holyc/cc.h>
#include <holyc/lex.h>

// Print to stderr and exit.
//
void error(char *fmt, ...) {
  __asm__("call _warnf");
  exit(1);
}

CC *cc_init(int argc, char **argv, char **envp, int input_size) {
  CC *cc             = malloc(sizeof(CC));
  cc->main_args.argc = argc;
  cc->main_args.argv = argv;
  cc->main_args.envp = envp;
  cc->code.start     = malloc(sizeof(char) * input_size);
  cc->code.curr      = cc->code.start;

  char *input = malloc(sizeof(char) * input_size);
  parse_new(&cc->parser, input, input_size);

  return cc;
}

int cc_read(CC *cc, int fd, int size) {
  Buffer input = cc->parser.lexer.input;
  input.size   = read(fd, input.curr, size);

  return input.size;
}
