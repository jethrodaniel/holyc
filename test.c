#include <lib/stdio.c>
#include <lib/stdbool.h>
#include <lib/stdlib.c>
#include <src/boot.c>

#define NUM_TESTS         1  // *Must* be kept up to date
#define TEST_NAME_MAX_LEN 16
#define TEST_ERR_MAX_LEN  16

// A single test.
//
typedef struct Test {
  char name[TEST_NAME_MAX_LEN];
  bool passed;
  char err[TEST_ERR_MAX_LEN];
} Test;

// The test suite's context.
//
typedef struct TestRunner {
  int argc;              // argc from main()
  char **argv;           // argv from main()
  bool verbose;          // show test name, or just '.'
  Test tests[NUM_TESTS]; // tests to run
} T;

//

char *test_exits_with_number(T *t) {
  if (t->verbose) printf("%s: ", __func__);

  int n = 42;
  return 0;
}

void run_test(char *(*fn)(), T *t) {
  char *msg = fn(t);

  if (msg)
    printf("%s", msg);
  else
    printf(".");
}

// Run all tests.
//
void run_tests(T *t) {
  run_test(test_exits_with_number, t);
  printf("\n");
}

//--

// Parse options, update test runner.
//
void parse_options(T *t) {
  for (int i = 0; i < t->argc; i++) {
    char *arg = t->argv[i];

    if (*arg == '-') arg++;
    else             continue;

    if (*arg != 'v') {
      warnf("unknown option -%c\n", *arg);
      exit(2);
    }

    warnf("verbose\n");
    t->verbose = true;
  }
}

// Driver program; parse options, run all tests.
//
int main(int argc, char **argv) {
  T *t = malloc(sizeof(T));
  t->verbose = false;
  t->argc = argc;
  t->argv = argv;
  run_tests(t);
}
