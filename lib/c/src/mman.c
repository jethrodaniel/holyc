#include <mman.h>

void *mmap(void *addr, int64_t length, int prot, int flags, int fd,
           off_t offset) {
  return (void *)syscall((int64_t)addr, length, prot, flags, fd, offset,
                         SYSCALL_MMAP);
}

int64_t munmap(void *addr, size_t length) {
  return syscall((int64_t)addr, length, 0, 0, 0, 0, SYSCALL_MUNMAP);
}
