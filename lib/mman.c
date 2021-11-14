#ifndef HOLYC_LIB_MMAN
#define HOLYC_LIB_MMAN

#include <lib/stdint.h>

#define PROT_READ     0x1
#define PROT_WRITE    0x2
#define PROT_EXEC     0x4
#define MAP_ANONYMOUS 0x20
#define MAP_PRIVATE   0x02

void *mmap(void *addr, int64_t length, int prot, int flags, int fd, off_t offset) {
  asm("mov $9, %rax");   // mmap
  asm("mov %rcx, %r10"); // arg4 for syscalls
  asm("syscall");
}

int munmap(void *addr, size_t length) {
  asm("mov $11, %rax");   // munmap
  asm("syscall");
}

#endif // HOLYC_LIB_MMAN
