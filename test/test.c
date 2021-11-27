// Small test framework based on Go lang's `testing` pkg:
// https://pkg.go.dev/testing

//--

#include <lib/crt0.c>
#include <lib/stdbool.h>
#include <lib/stdio.c>
#include <lib/stdlib.c>
#include <lib/string.c>

#define TEST_NAME_MAX_LEN 64
#define TEST_ERR_MAX_LEN 64

typedef void (*TestFn)();

// A single test.
//
typedef struct Test {
  TestFn fn;
  char   name[TEST_NAME_MAX_LEN];
  bool   passed;
  char  *err[TEST_ERR_MAX_LEN];
} Test;

// The test suite's context.
//
typedef struct TestRunner {
  int    argc;      // argc from main()
  char **argv;      // argv from main()
  bool   verbose;   // verbose output?
  int    num_tests; // number of tests
  Test   tests[];   // tests to run
} T;

// Parse options, update test runner.
//
void parse_options(T *t) {
  for (int i = 0; i < t->argc; i++) {
    char *arg = t->argv[i];

    if (*arg == '-')
      arg++;
    else
      continue;

    if (*arg != 'v') {
      warnf("unknown option -%c\n", *arg);
      exit(2);
    }

    t->verbose = true;
  }
}

// Setup a new test context
//
T *test_init(Test *tests, int num_tests, int argc, char **argv) {
  T *t = malloc(sizeof(T));
  t->verbose = false;
  t->argc = argc;
  t->argv = argv;
  t->num_tests = num_tests;

  for (int i = 0; i < t->num_tests; i++)
    t->tests[i] = tests[i];

  parse_options(t);

  return t;
}

// Run a single test.
//
bool run_test(T *t, int i) {
  Test *test = &t->tests[i];

  if (t->verbose) {
    printf("==> %s\n", test->name);

    test->fn(test);

    if (test->passed)
      printf("  ok\n", test->name);
    else
      printf("  err\n", test->name);
  } else {
    test->fn(test);

    if (test->passed)
      printf(".");
    else
      printf("x");
  }

  return test->passed;
}

// Run all tests.
//
int run_tests(T *t) {
  bool failed = false;

  for (int i = 0; i < t->num_tests; i++)
    if (!run_test(t, i))
      failed = true;

  if (!t->verbose)
    printf("\n");

  if (failed)
    printf("ERR\n");
  else
    printf("OK\n");

  return failed ? 1 : EXIT_SUCCESS;
}
