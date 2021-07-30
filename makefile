PROG = holyc

CC := gcc

CFLAGS += -O0                         # simple code output
CFLAGS += -e _start                   # entry-point
CFLAGS += -static                     # don't link against shared libraries?
CFLAGS += -nostdlib                   # don't link any libs or startup files
CFLAGS += -ffreestanding              # disable builtin functions
CFLAGS += -mincoming-stack-boundary=4 # ensure 16-byte alignment
CFLAGS += -masm=intel                 # use Intel assembly syntax

# SysV requires 128-byte redzone, but kernel code can't use it
# CFLAGS += -mno-red-zone

# SysV requires .eh_frame stuff?
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
