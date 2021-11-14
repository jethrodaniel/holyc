UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  NASM_FLAGS := -f elf64
endif
ifeq ($(UNAME), Darwin)
  LIBC_LDFLAGS := -L$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/lib -lSystem
  NASM_FLAGS := -f macho64
endif

LDFLAGS := -static
LDFLAGS += -e _start

CFLAGS := -g
CFLAGS += -O0
CFLAGS += -static
CFLAGS += -nostdlib
CFLAGS += -ffreestanding
CFLAGS += -fno-stack-protector

# asm stubs return via `rax`, not `return`
CFLAGS += -Wno-return-type

# clang ignores this for inline asm
# CFLAGS += -masm=intel

CFLAGS += -I ..

default: hello libc main clean

hello: hello.s
	nasm $(NASM_FLAGS) $<
	ld $(LDFLAGS) -o $@ $@.o
	./$@

libc: libc.s
	nasm $(NASM_FLAGS) $<
	ld $(LIBC_LDFLAGS) -o $@ $@.o
	./$@

main: main.s main.c
	nasm $(NASM_FLAGS) -o start.o main.s
	gcc $(CFLAGS) -c main.c
	ld $(LDFLAGS) -o $@ start.o main.o
	./$@

clean:
	rm -fv *.o hello libc main
