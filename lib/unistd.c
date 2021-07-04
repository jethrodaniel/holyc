#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define EXIT_SUCCESS  0

int write(int fd, char *buf, int length) {
  asm("mov  rax, 1");
  asm("syscall");
}

int read(int fd, char *buf, int length) {
  asm("mov  rax, 0");
  asm("syscall");
}
