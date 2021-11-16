.global _start
.extern _main,_exit

.text

_start:
; popq %rdi
; movq %rsp, %rsi
; andq $0xfffffffffffffff0, %rsp
call _main
movq %rax, %rdi
call _exit
