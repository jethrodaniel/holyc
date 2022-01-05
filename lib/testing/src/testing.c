#include <testing.h>

void assert(Test *t, bool expr, char *msg) {
  printf("  %s  %s\n", expr ? "." : "x", msg);
  if (expr)
    return;
  t->passed = false;
}

// Setup a new test context
//
T *test_init(Test *tests, int num_tests, int argc, char **argv) {
  T *t = malloc(sizeof(T));
  t->argc = argc;
  t->argv = argv;
  t->num_tests = num_tests;

  for (int i = 0; i < t->num_tests; i++) {
    t->tests[i] = tests[i];
    t->tests[i].passed = true;
  }

  return t;
}

// Run a single test.
//
bool run_test(T *t, int i) {
  Test *test = &t->tests[i];
  printf("-- %s\n", test->name);

  test->fn(test);

  return test->passed;
}

// Run all tests.
//
int run_tests(T *t) {
  int failed = 0;

  for (int i = 0; i < t->num_tests; i++)
    if (!run_test(t, i))
      failed++;

  if (failed)
    printf("ERR\n");
  else
    printf("OK\n");

  return failed;
}
