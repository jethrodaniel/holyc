#include "lib/boot.c"
#include "lib/stddef.h"
#include "lib/stdint.h"
#include "lib/unistd.c"
#include "lib/string.c"
#include "lib/mman.c"
#include "lib/stdlib.c"
#include "lib/stdio.c"
#include "lib/stdbool.h"

typedef struct Options {
  bool verbose;
} Options;

//

char *test_exits_with_number(Options *opts) {
  if (opts->verbose)
    printf("%s: ", __func__);

  int n = 42;
  return 0;
}

void run_test(char *(*fn)(), Options *opts) {
  char *msg = fn(opts);

  if (msg)
    printf("%s", msg);
  else
    printf(".");
}

void run_tests(Options *opts) {
  run_test(test_exits_with_number, opts);
  printf("\n");
}

//

int main(int argc, char **argv) {
  Options *opts = malloc(sizeof(Options));
  opts->verbose = false;

  for (int i = 0; i < argc; i++) {
    char *arg = argv[i];

    if (*arg == '-')
      arg++;
    else
      continue;

    if (*arg == 'v') {
      warnf("verbose\n");
      opts->verbose = true;
    } else {
      warnf("unknown option -%c\n", *arg);
      exit(2);
    }
  }

  run_tests(opts);
}
