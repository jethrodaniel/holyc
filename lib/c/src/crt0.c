extern int main();
extern int exit(int code);

__attribute__((naked)) void start() {
  __asm__("movq  (%rsp),  %rdi");         // argc
  __asm__("leaq  8(%rsp), %rsi");         // argv
  __asm__("leaq  16(%rsp,%rdi,8), %rdx"); // envp
  __asm__("call  _main");                 // main(argc, argv, envp)

  __asm__("mov   %rax, %rdi"); // status
  __asm__("call  _exit");      // exit(status)
}
