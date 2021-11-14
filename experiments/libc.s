section .data
    msg db "hi! (libc)", 10, 0

section .text

global _main
extern _exit, _puts

; stack must be 16-byte aligned at time of fn calls
;
; When `_main` is called, it's 8-byte return address is on the stack, so
; we subtract 8 bytes at the beginning to align the stack, and then add
; those bytes back before returning.
;
_main:
  sub rsp, 8           ; align rsp to 16-bytes

  lea rdi, [rel msg]   ; msg
  call _puts           ; puts(msg)

  mov rdi, 0           ; status
  call _exit           ; exit(status)

  add rsp, 8           ; restore rsp

  mov rax, 0           ; return 0
  ret
