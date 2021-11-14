LIBC_LDFLAGS := -L$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/lib -lSystem

LDFLAGS := -static
LDFLAGS += -e _start

CFLAGS += -static
CFLAGS += -nostdlib
CFLAGS += -ffreestanding
CFLAGS += -masm=intel

default: hello libc clean

hello: hello.s
	nasm -f macho64 $<
	ld $(LDFLAGS) -o $@ $@.o
	./$@

libc: libc.s
	nasm -f macho64 $<
	ld $(LIBC_LDFLAGS) -o $@ $@.o
	./$@

main: main.s main.c
	nasm -f macho64 -o start.o main.s
	gcc $(CFLAGS) -c main.c
	ld $(LDFLAGS) -o $@ start.o main.o
	./$@

clean:
	rm -fv *.o hello libc main
