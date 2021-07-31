#include <lib/stdio.c>
#include <lib/stdbool.h>
#include <lib/stdlib.c>
#include <lib/string.c>
#include <src/boot.c>

#define NUM_TESTS         3  // *Must* be kept up to date
#define TEST_NAME_MAX_LEN 64
#define TEST_ERR_MAX_LEN  64

typedef void (*TestFn)();

// A single test.
//
typedef struct Test {
  char name[TEST_NAME_MAX_LEN];
  TestFn fn;
  bool passed;
  char *err[TEST_ERR_MAX_LEN];
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

void test_pass(Test *t) {
  t->passed = true;
}

void test_fail(Test *t) {
  char *msg = "failed";
  memcpy(t->err, msg, strlen(msg));
}

void run_test(T *t, Test *test) {
  test->fn(test);
  if (t->verbose) printf("%s: ", test->name);

  if (test->passed) return printf(".\n");

  if (t->verbose) printf("%s\n", test->err);
  else            printf("x\n");
}

// Run all tests.
//
void run_tests(T *t) {
  for (int i = 0; i < NUM_TESTS; i++)
    run_test(t, &t->tests[i]);
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
  t->tests[0] = (Test){.name="test_pass", .fn=test_pass};
  t->tests[1] = (Test){.name="test_fail", .fn=test_fail};
  t->tests[2] = (Test){.name="test_fail", .fn=test_fail};

  parse_options(t);

  run_tests(t);

  return EXIT_SUCCESS;
}
