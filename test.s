bits 64

push 1
pop rax
mov rdi, rax
mov rax, strict 0x2000001
syscall
