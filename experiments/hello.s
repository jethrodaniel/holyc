; nasm -f macho64 64.asm && ld -macosx_version_min 10.7.0 -lSystem -o 64 64.o && ./64
;
; From https://gist.github.com/FiloSottile/7125822

; global start
global _main

section .text

; start:
_main:
    mov     rax, 0x2000004 ; write
    mov     rdi, 1 ; stdout
    mov     rsi, msg
    mov     rdx, msg.len
    syscall

    mov     rax, 0x2000001 ; exit
    mov     rdi, 0
    syscall


section .data

msg:    db      "Hello, world!", 10
.len:   equ     $ - msg
