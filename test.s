bits 64

push 4
push 2
pop rax
pop rdi
imul rax,rdi
cqo
idiv rdi
