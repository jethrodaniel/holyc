int exit(int code) {
  // asm("mov rax, 0x2000060");
  asm("mov $0x2000001, %rax"); // exit
  asm("syscall");
}

int main(int argc, char **argv, char **envp) {
  exit(24);
  return 42;
}
