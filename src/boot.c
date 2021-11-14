#ifndef HOLYC_LIB_BOOT
#define HOLYC_LIB_BOOT

asm(".globl _start");
asm("_start:");
  // asm("mov   rdi, QWORD PTR [rsp]"); // argc
  asm("movq  (%rsp), %rdi");         // argc
  // asm("lea   rsi,[rsp+0x8]");        // argv
  asm("leaq    8(%rsp), %rsi");      // argv
  // asm("lea   rdx,[rsp+rdi*8+0x10]"); // envp
  asm("leaq    16(%rsp,%rdi,8), %rdx"); // envp
  // asm("call  main");                 // main(argc, argv, envp)
  asm("call  _main");                 // main(argc, argv, envp)

  // asm("mov   rdi,rax"); // status
  asm("mov   %rax, %rdi"); // status
  // asm("mov   rax, 60"); // exit
  asm("mov   $60, %rax"); // exit
  // asm("call  exit");    // exit(status)
  asm("call  _exit");    // exit(status)

#endif // HOLYC_LIB_BOOT
