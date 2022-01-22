PROG = holyc

# CC := gcc
#
UNAME := $(shell uname)

#--

CFLAGS += -g

# simple code output
CFLAGS += -O0

# output standalone executable
CFLAGS += -static

# don't link any libs or startup files
CFLAGS += -nostdlib

# disable builtin functions
#
# Note that it is assumed that a freestanding environment will additionally
# provide memcpy, memmove, memset and memcmp implementations, as these are
# needed for efficient codegen for many programs.
#
CFLAGS += -ffreestanding

# ensure 16-byte alignment, required by SysV | TODO: any of this needed?
ifeq ($(UNAME), Linux)
  CFLAGS += -mincoming-stack-boundary=4

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

#-- warnings

CFLAGS += -Wall
CFLAGS += -Werror

# clang yells at us for using `-e` with `-c`...
# CFLAGS += -Wno-unused-command-line-argument

# .eh_frame stuff?, required by SysV
# TODO: is this needed?
# CFLAGS += -fno-asynchronous-unwind-tables

# Allow for #include </lib/...> instead of relative paths
CFLAGS += -I . -I lib/c/include -I include

#--

default: clean test

SRCS       := $(wildcard src/*.c)
OBJS       := $(SRCS:.c=.o)
LIBC       := lib/c/libc.a
LIBC_FLAGS := -Llib/c -lc

$(LIBC):
	$(MAKE) -C lib/c

$(PROG): $(OBJS) | $(LIBC)
	$(CC) -e _start $(CFLAGS) $(LIBC_FLAGS) $^ $(LIBC) -o $(PROG)

clean:
	rm -f $(OBJS) $(PROG) *.out test/*.o $(TEST_MAIN)
	$(MAKE) -C lib/c clean

#--
test/bin:
	mkdir -p $@
test/bin/lex: test/lex.o src/lex.o $(LIBC) | test/bin
	$(CC) -e _start $(CFLAGS) $(LIBC_FLAGS) $^ -o $@
	./$@
test/bin/parse: test/parse.o $(filter-out src/main.o, $(OBJS)) $(LIBC) | test/bin
	$(CC) -e _start $(CFLAGS) $(LIBC_FLAGS) $^ -o $@
	./$@
test/bin/compile: test/compile.o $(filter-out src/main.o, $(OBJS)) $(LIBC) | test/bin
	$(CC) -e _start $(CFLAGS) $(LIBC_FLAGS) $^ -o $@
	./$@

test: test/bin/lex test/bin/parse test/bin/compile $(PROG) FORCE
	sh test/main.sh
FORCE:

lint: $(SRCS)
	clang-format -i $^

#--

docker: docker-build docker-run
docker-build:
	docker build -t holyc .
docker-run:
	docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined holyc
