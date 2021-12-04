#ifndef HOLYC_LIB_UNISTD
#define HOLYC_LIB_UNISTD

#define STDIN_FILENO  0
#define STDIN         STDIN_FILENO
#define STDOUT_FILENO 1
#define STDOUT        STDOUT_FILENO
#define STDERR_FILENO 2
#define EXIT_SUCCESS  0

#include <lib/stdint.h>
#include <lib/syscall.c>

int64_t write(int fd, void *buf, size_t length) {
  return syscall(fd, buf, length, 0, 0, 0, SYSCALL_WRITE);
}

int64_t read(int fd, char *buf, size_t length) {
  return syscall(fd, buf, length, 0, 0, 0, SYSCALL_READ);
}

typedef int pid_t;

pid_t getpid() {
  return syscall(0, 0, 0, 0, 0, 0, SYSCALL_GETPID);
}

pid_t fork() {
  pid_t curr = getpid();

  pid_t pid = syscall(0, 0, 0, 0, 0, 0, SYSCALL_FORK);

  if (curr == pid)
    return 0;

  return pid;
}

#endif // HOLYC_LIB_UNISTD
