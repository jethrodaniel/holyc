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
LDFLAGS += -L ../lib/c -lc

# CFLAGS := -g
CFLAGS += -O0
CFLAGS += -static
CFLAGS += -nostdlib
CFLAGS += -ffreestanding
CFLAGS += -fno-stack-protector
CFLAGS += -Wno-return-type
CFLAGS += -I ..
CFLAGS += -mstack-alignment=4

LIBC   := lib/c/libc.a

default: bin hello libc main test clean $(LIBC)

$(LIBC):
	$(MAKE) -C ../lib/c

bin:
	mkdir -p bin

hello: hello.s bin
	nasm $(NASM_FLAGS) $<
	ld $(LDFLAGS) -o bin/$@ $@.o
	./bin/$@

libc: libc.s bin
	nasm $(NASM_FLAGS) $<
	ld $(LIBC_LDFLAGS) -o bin/$@ $@.o
	./bin/$@

main: main.c bin
	cc -e _start $(CFLAGS) $(LDFLAGS) $< -o bin/$@
	./bin/$@ || exit 0

test: test.s bin
	nasm -f bin $< -o bin/$@ && ndisasm -b64 bin/$@

clean:
	rm -rfv bin *.o
