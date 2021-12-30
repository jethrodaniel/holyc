#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  printf("[TEST] %s\n", __FILE__);

  pid_t pid;

  pid = fork();
  printf("pid: %d\n", pid);

  if (pid == 0) {
    print("hello from parent!\n");
  } else {
    printf("hello from child! pid: %d\n", pid);
    exit(3);
  }

  return 0;
}
