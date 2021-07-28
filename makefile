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
