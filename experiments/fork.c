#include "../lib/crt0.c"
#include "../lib/stdio.c"

int main(int argc, char **argv, char **envp) {
  printf("== fork example ==\n");

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
