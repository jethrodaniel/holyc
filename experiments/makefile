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

default: bin hello libc main macho clean

bin:
	mkdir -p bin

hello: hello.s
	nasm $(NASM_FLAGS) $<
	ld $(LDFLAGS) -o bin/$@ $@.o
	./bin/$@

libc: libc.s
	nasm $(NASM_FLAGS) $<
	ld $(LIBC_LDFLAGS) -o bin/$@ $@.o
	./bin/$@

main: main.c
	cc -e _start $(CFLAGS) $< -o bin/$@
	./bin/$@

macho: macho.c
	cc -e _start $(CFLAGS) $< -o bin/$@
	./bin/$@ || true

clean:
	rm -rfv bin *.o
