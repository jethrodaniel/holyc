PROG = holyc

CC := gcc
CFLAGS = -O0 -static -nostdlib -ffreestanding -e _start

# https://stackoverflow.com/a/16007194/7132678
CFLAGS += -fno-asynchronous-unwind-tables

# abi conradicts the CPU?
CFLAGS += -mno-red-zone

# https://stackoverflow.com/questions/2548486/compiling-without-libc?noredirect=1&lq=1#comment109923208_2548601
CFLAGS += -mincoming-stack-boundary=4

CFLAGS += -masm=intel

default: clean $(PROG) run
disasm: $(PROG)
	dd skip=120 bs=1 if=./$< 2> /dev/null | ndisasm -b64 -
run: $(PROG)
test: force
	./test
force:
clean:
	rm -vf a.out $(PROG) *.o main *.s
$(PROG): hc.c
	$(CC) $(CFLAGS) $< -o $(PROG)
asm: hc.c
	$(CC) $(CFLAGS) -S $< -o $(PROG).s
