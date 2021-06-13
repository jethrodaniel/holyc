; syscall table: https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

bits 64

_start:
   mov rax, 60  ; exit
   mov rdi, 42  ; arg1, exit code
   syscall      ; exit()
