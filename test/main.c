#include <holyc/cc.h>
#include <holyc/lex.h>

#include <testing.h>

int main(int argc, char **argv) {
  Test tests[] = {
      // {test_cc_setup, "test_cc_setup"},
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
