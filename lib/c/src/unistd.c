#include <unistd.h>

int64_t write(int fd, void *buf, size_t length) {
  return syscall(fd, (int64_t)buf, length, 0, 0, 0, SYSCALL_WRITE);
}
int64_t read(int fd, char *buf, size_t length) {
  return syscall(fd, (int64_t)buf, length, 0, 0, 0, SYSCALL_READ);
}

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
