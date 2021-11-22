PROG = holyc

# CC := gcc
#
UNAME := $(shell uname)

#--

# simple code output
CFLAGS += -O0

# entry-point
CFLAGS += -e _start

# clang yells at us for using `-e` with `-c`...
CFLAGS += -Wno-unused-command-line-argument

# output standalone executable
CFLAGS += -static

# don't link any libs or startup files
CFLAGS += -nostdlib

# disable builtin functions
CFLAGS += -ffreestanding

# ensure 16-byte alignment, required by SysV | TODO: any of this needed?
ifeq ($(UNAME), Linux)
  # CFLAGS += -mincoming-stack-boundary=4

  # Ensure we can still `call` without having to check if `__APPLE__`
  CFLAGS += -fleading-underscore
endif
ifeq ($(UNAME), Darwin)
  # CFLAGS += -mstack-alignment=4

  # Needed to prevent:
  #     Undefined symbols for architecture x86_64:
  #       "___stack_chk_fail", referenced from:
  #           __printf_print_itoa in hc.o
  #           __warnf_print_itoa in hc.o
  #           _main in hc.o
  #       "___stack_chk_guard", referenced from:
  #           __printf_print_itoa in hc.o
  #           __warnf_print_itoa in hc.o
  #           _main in hc.o
  CFLAGS += -fno-stack-protector
endif

# 128-byte redzone, required by SysV (kernel code can't use it)
CFLAGS += -mno-red-zone

# asm stubs return via `rax`, not `return`
CFLAGS += -Wno-return-type

# don't yell at me
CFLAGS += -Wno-int-conversion

# .eh_frame stuff?, required by SysV
# TODO: is this needed?
# CFLAGS += -fno-asynchronous-unwind-tables

# Allow for #include </lib/...> instead of relative paths
CFLAGS += -I .

#--

default: clean $(PROG) test

SRCS = $(wildcard src/*.c lib/*.c)
OBJS = $(SRCS:.c=.o)

$(PROG): src/main.o $(OBJS)
	$(CC) $(CFLAGS) $< -o $(PROG)

clean:
	rm -vf *.o *.out $(PROG) src/*.o lib/*.o out
	# make -C experiments clean

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
	docker build -t holyc . && docker run -it --rm holyc
