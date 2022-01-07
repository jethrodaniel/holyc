#include <holyc/cc.h>
#include <holyc/lex.h>

#include <testing.h>

void test_cc_setup(Test *t) {
  char *argv[] = {
      "progname", //
      "arg1"      //
  };
  CC *cc = cc_init(2, argv, argv, 1024);

  assert(t, cc->parser.current.type == 0, "initial token should be EOF");
}

void test_fail(Test *t) {
  t->passed = false;
}

int main(int argc, char **argv) {
  Test tests[] = {
      {test_cc_setup, "test_cc_setup"},
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
