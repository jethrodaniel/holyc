#include "test.c"

void test_pass(Test *t) {
  t->passed = true;
}

void test_fail(Test *t) {
  char *msg = "failed";
  memcpy(t->err, msg, strlen(msg));
}

int main(int argc, char **argv) {
  Test tests[] = {
    {test_pass, "test_pass"},
    // {test_fail, "test_fail"},
    {test_pass, "test_pass"},
  };

  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
