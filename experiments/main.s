global _start
extern _main

section .text

_start:
    ; sub rsp, 8          ; align rsp to 16-bytes

    call _main

    ; add rsp, 8          ; restore rsp

    mov rdi, rax
    mov rax, 0x2000001  ; exit
    syscall
