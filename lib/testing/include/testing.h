#ifndef TESTING_H
#define TESTING_H

// Small test framework based on Go lang's `testing` pkg:
// https://pkg.go.dev/testing

//--

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_NAME_MAX_LEN 64

typedef void (*TestFn)();

// A single test.
//
typedef struct Test {
  TestFn fn;
  char   name[TEST_NAME_MAX_LEN];
  bool   passed;
} Test;

// The test suite's context.
//
typedef struct TestRunner {
  int    argc;      // argc from main()
  char **argv;      // argv from main()
  int    num_tests; // number of tests
  Test   tests[];   // tests to run
} T;

void assert(Test *t, bool expr, char *msg);
T   *test_init(Test *tests, int num_tests, int argc, char **argv);
bool run_test(T *t, int i);
int  run_tests(T *t);

#endif // TESTING_H
