PROG = holyc

CC := gcc
CFLAGS = -O0 -static -nostdlib -ffreestanding

# https://stackoverflow.com/a/16007194/7132678
CFLAGS += -fno-asynchronous-unwind-tables

# abi conradicts the CPU?
CFLAGS += -mno-red-zone

# CFLAGS += -masm=intel

default: clean $(PROG) run
disasm: a.out
	dd skip=120 bs=1 if=./$< 2> /dev/null | ndisasm -b64 -
run: a.out
	echo hi | ./holyc
# ./$< ; echo $$?
a.out: $(PROG)
	perl -e 'print "\xB8<\x00\x00\x00\xBF*\x00\x00\x00\x0F\x05\n"' | ./$(PROG) > $@ && chmod u+x $@
test: force
	./test
force:
clean:
	rm -vf a.out $(PROG) *.o main *.s
$(PROG): hc.c
	$(CC) $(CFLAGS) $< -o $(PROG)
asm: hc.c
	$(CC) $(CFLAGS) -S $< -o $(PROG).s
