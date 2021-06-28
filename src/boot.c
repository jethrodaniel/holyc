asm(".globl _start");
asm("_start:");
  asm("mov    rdi, QWORD PTR [rsp]"); // argc
  asm("lea    rsi,[rsp+0x8]");        // argv
  asm("lea    rdx,[rsp+rdi*8+0x10]"); // envp
  asm("call   main");                 // main(argc, argv, envp)

  asm("mov    rdi,rax"); // status
  asm("mov    rax, 60"); // exit
  asm("call   exit");    // exit(status)

