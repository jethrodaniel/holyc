#ifndef HOLYC_LIB_UNISTD
#define HOLYC_LIB_UNISTD

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define EXIT_SUCCESS 0

int write(int fd, char *buf, int length) {
#ifdef __APPLE__
  __asm__("mov $0x2000004, %rax");
#else
  __asm__("mov $1, %rax");
#endif
  __asm__("syscall");
}

int read(int fd, char *buf, int length) {
#ifdef __APPLE__
  __asm__("mov $0x2000000, %rax");
#else
  __asm__("mov $0, %rax");
#endif
  __asm__("syscall");
}

#endif // HOLYC_LIB_UNISTD
