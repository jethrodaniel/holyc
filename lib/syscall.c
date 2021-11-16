#ifndef HOLYC_LIB_SYSCALL
#define HOLYC_LIB_SYSCALL

#ifdef __APPLE__
  #define SYSCALL_BASE 0x2000000
#else
  #define SYSCALL_BASE 0x0
#endif
#define SYSCALL_GET(num) SYSCALL_BASE + num

#include <lib/stdint.h>

int64_t syscall(int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4, int64_t arg5, int64_t arg6, int64_t n);

__asm__("_syscall:");

__asm__("movq %rcx, %r10");     // 4th arg for syscall is different
__asm__("movq 8(%rsp), %rax");  // syscall number
__asm__("pushq %rbx");          // alignment
__asm__("syscall");
__asm__("popq %rbx");
__asm__("retq");
__asm__("nopl (%rax)");

#endif // HOLYC_LIB_SYSCALL
