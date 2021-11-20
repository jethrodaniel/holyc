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

# CFLAGS := -g
CFLAGS += -O0
CFLAGS += -static
CFLAGS += -nostdlib
CFLAGS += -ffreestanding
CFLAGS += -fno-stack-protector
CFLAGS += -Wno-return-type
CFLAGS += -I ..
CFLAGS += -mstack-alignment=4

default: hello libc main macho clean

hello: hello.s
	nasm $(NASM_FLAGS) $<
	ld $(LDFLAGS) -o $@ $@.o
	./$@

libc: libc.s
	nasm $(NASM_FLAGS) $<
	ld $(LIBC_LDFLAGS) -o $@ $@.o
	./$@

main: main.c
	cc -e _start $(CFLAGS) $< -o $@
	./$@

macho: macho.c
	cc -e _start $(CFLAGS) $< -o $@
	./$@

clean:
	rm -fv *.o hello libc main macho
