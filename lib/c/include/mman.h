#ifndef HOLYC_LIB_C_MMAN
#define HOLYC_LIB_C_MMAN

#include <stdint.h>

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

#include <stdint.h>
#include <syscall.h>

void *mmap(void *addr, int64_t length, int prot, int flags, int fd,
           off_t offset);

int64_t munmap(void *addr, size_t length);

#endif // HOLYC_LIB_C_MMAN
