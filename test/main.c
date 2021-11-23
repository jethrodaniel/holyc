#include "setup.c"

void test_pass(Test *t) {
  t->passed = true;
}

void test_fail(Test *t) {
  char *msg = "failed";
  memcpy(t->err, msg, strlen(msg));
}

#define TEST(fn, desc) {fn, #fn " - " desc},

static Test tests[] = {
  TEST(test_pass, "this one passes")
  // TEST(test_fail, "this one fails")
  TEST(test_pass, "this one passes")
};


int main(int argc, char **argv) {
  T *t = test_init(tests, sizeof(tests) / sizeof(Test), argc, argv);
  return run_tests(t);
}
