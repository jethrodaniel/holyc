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
#ifdef __APPLE__
  return syscall(fd, buf, length, 0, 0, 0, SYSCALL_GET(4));
#else
  return syscall(fd, buf, length, 0, 0, 0, SYSCALL_GET(1));
#endif
}

int64_t read(int fd, char *buf, size_t length) {
#ifdef __APPLE__
  return syscall(fd, buf, length, 0, 0, 0, SYSCALL_GET(3));
#else
  return syscall(fd, buf, length, 0, 0, 0, SYSCALL_GET(0));
#endif
}

#endif // HOLYC_LIB_UNISTD
