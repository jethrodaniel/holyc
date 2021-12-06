#ifndef HOLYC_LIB_MMAN
#define HOLYC_LIB_MMAN

#include <lib/stdint.h>

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

#ifdef __APPLE__
#define MAP_ANONYMOUS 0x1000
#else
#define MAP_ANONYMOUS 0x20
#endif

#define MAP_PRIVATE 0x02
#define MAP_FAILED  ((void *)-1)

#include <lib/syscall.c>

void *mmap(void *addr, int64_t length, int prot, int flags, int fd,
           off_t offset) {
  return (void *)syscall((int64_t)addr, length, prot, flags, fd, offset, SYSCALL_MMAP);
}

int64_t munmap(void *addr, size_t length) {
  return syscall((int64_t)addr, length, 0, 0, 0, 0, SYSCALL_MUNMAP);
}

#endif // HOLYC_LIB_MMAN
