#include "../lib/stdlib.c"
#include "../lib/stdio.c"

int main(int argc, char **argv, char **envp) {
  // write(1, "hi!\n", 5);
  print("print!\n");
  int i = 142;
  printf("printf!\n");
  printf("i = %d!\n", i);
  exit(24);
  return 42;
}
