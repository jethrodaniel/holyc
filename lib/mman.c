#ifndef HOLYC_LIB_MMAN
#define HOLYC_LIB_MMAN

#include <lib/stdint.h>

#define PROT_READ     0x1
#define PROT_WRITE    0x2
#define PROT_EXEC     0x4

#ifdef __APPLE__
  #define MAP_ANONYMOUS 0x1000
#else
  #define MAP_ANONYMOUS 0x20
#endif

#define MAP_PRIVATE   0x02
#define MAP_FAILED ((void *) - 1 )

#include <lib/syscall.c>

// void *mmap(void *addr, int64_t length, int prot, int flags, int fd, off_t offset) {
void *mmap(void *addr, long long length, long long prot, long long flags, long long fd, long long offset) {
#ifdef __APPLE__
  return syscall(addr, length, prot, flags, fd, offset, SYSCALL_GET(197));
#else
  return syscall(addr, length, prot, flags, fd, offset, SYSCALL_GET(9));
#endif
}

int64_t munmap(void *addr, size_t length) {
#ifdef __APPLE__
  return syscall(addr, length, 0, 0, 0, 0, SYSCALL_GET(73));
#else
  return syscall(addr, length, 0, 0, 0, 0, SYSCALL_GET(11));
#endif
}

#endif // HOLYC_LIB_MMAN
