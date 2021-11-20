#ifndef HOLYC_LIB_BOOT
#define HOLYC_LIB_BOOT

__asm__(".globl _start");
__asm__(".extern _main, _exit");

__asm__("_start:");

__asm__("movq  (%rsp),  %rdi");         // argc
__asm__("leaq  8(%rsp), %rsi");         // argv
__asm__("leaq  16(%rsp,%rdi,8), %rdx"); // envp
__asm__("call  _main");                 // main(argc, argv, envp)

__asm__("mov   %rax, %rdi");            // status
__asm__("call  _exit");                 // exit(status)

#endif // HOLYC_LIB_BOOT
