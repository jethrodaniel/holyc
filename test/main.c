#include "test.c"
#include <holyc/cc.h>

void test_cc_setup(Test *t) {
  char *argv[] = {
      "progname", //
      "arg1"      //
  };
  CC *cc = cc_init(2, argv, argv, 1024);

  assert(t, cc->token == 0, "initial token should be EOF");
}

void test_fail(Test *t) {
  char *msg = "failed";
  memcpy(t->msg, msg, strlen(msg));
  t->passed = false;
}

int main(int argc, char **argv) {
  Test tests[] = {
      {test_cc_setup, "test_cc_setup"},
    // {test_fail,     "test_fail"    },
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
