PROG = holyc

CC := gcc

# simple code output
CFLAGS += -O0

# entry-point
CFLAGS += -e _start

# use Intel assembly syntax
CFLAGS += -masm=intel

# output standalone executable
CFLAGS += -static

# don't link any libs or startup files
CFLAGS += -nostdlib

# disable builtin functions
CFLAGS += -ffreestanding

# ensure 16-byte alignment, required by SysV
CFLAGS += -mincoming-stack-boundary=4

# 128-byte redzone, required by SysV (kernel code can't use it)
CFLAGS += -mno-red-zone

# .eh_frame stuff?, required by SysV
# TODO
# CFLAGS += -fno-asynchronous-unwind-tables


default: clean $(PROG) test

$(PROG): hc.c
	$(CC) $(CFLAGS) $< -o $(PROG)

# test.out: $(PROG) FORCE test.c
# 	$(CC) $(CFLAGS) test.c -o $@
# ctest: test.out
# 	./$< -v
test: FORCE
	sh test.sh

FORCE:

clean:
	rm -vf *.out $(PROG) *.o main

#--

docker:
	docker build -t holyc .
