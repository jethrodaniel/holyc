#ifndef HOLYC_LIB_C_SYSCALL
#define HOLYC_LIB_C_SYSCALL

#ifdef __APPLE__
#define SYSCALL_MMAP   0x2000000 + 197
#define SYSCALL_MUNMAP 0x2000000 + 73
#define SYSCALL_WRITE  0x2000000 + 4
#define SYSCALL_READ   0x2000000 + 3
#define SYSCALL_GETPID 0x2000000 + 20
#define SYSCALL_FORK   0x2000000 + 2
#define SYSCALL_EXIT   0x2000000 + 1
#else
#define SYSCALL_MMAP   9
#define SYSCALL_MUNMAP 11
#define SYSCALL_WRITE  1
#define SYSCALL_READ   0
#define SYSCALL_GETPID 39
#define SYSCALL_FORK   57
#define SYSCALL_EXIT   60
#endif

#include <stdint.h>

int64_t syscall(int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4,
                int64_t arg5, int64_t arg6, int64_t n);

__asm__("_syscall:");

__asm__("movq %rcx, %r10");    // 4th arg for syscall is different
__asm__("movq 8(%rsp), %rax"); // syscall number
__asm__("pushq %rbx");         // alignment
__asm__("syscall");
__asm__("popq %rbx");
__asm__("retq");

#endif // HOLYC_LIB_C_SYSCALL
