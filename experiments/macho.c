// Minimal main() to examine macho-o files.

__asm__(".globl _start");
__asm__("_start:");

__asm__("movq  (%rsp),  %rdi");         // argc
__asm__("leaq  8(%rsp), %rsi");         // argv
__asm__("leaq  16(%rsp,%rdi,8), %rdx"); // envp
__asm__("call  _main");                 // main(argc, argv, envp)

// __asm__("mov   %rax, %rdi");            // status
__asm__("mov   $42, %rdi");       // status
__asm__("mov  $0x2000001, %rax"); // exit
__asm__("syscall");               // exit status

int main(int argc, char **argv, char **envp) {
  return 42;
}
