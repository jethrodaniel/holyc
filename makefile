PROG = holyc

CC := gcc

#--

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
# CFLAGS += -mincoming-stack-boundary=4

# 128-byte redzone, required by SysV (kernel code can't use it)
CFLAGS += -mno-red-zone

CFLAGS += -fno-stack-protector

# .eh_frame stuff?, required by SysV
# TODO
# CFLAGS += -fno-asynchronous-unwind-tables

# Allow for #include </lib/...> instead of relative paths
CFLAGS += -I .

#--

default: clean $(PROG) test

SRCS = $(wildcard hc.c src/*.c lib/*.c)
OBJS = $(SRCS:.c=.o)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $< -o $(PROG)

clean:
	rm -vf *.out $(PROG)
purge: clean
	rm -vf *.o src/*.o lib/*.o

#--

test.out: $(PROG) FORCE test.c clean
	$(CC) $(CFLAGS) -g test.c -o $@
ctest: test.out
	./$< -v

test: FORCE
	sh test.sh
FORCE:

#--

docker:
	docker build -t holyc .
