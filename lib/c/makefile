PROG = holyc

# CC := gcc
#
UNAME := $(shell uname)

#--

CFLAGS += -e _start
CFLAGS += -static
CFLAGS += -nostdlib
CFLAGS += -ffreestanding

ifeq ($(UNAME), Linux)
  CFLAGS += -mincoming-stack-boundary=4
  CFLAGS += -fleading-underscore
endif
ifeq ($(UNAME), Darwin)
  CFLAGS += -fno-stack-protector
  SO_FLAGES := -shared
  SO_FLAGES += -L$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/lib -lSystem
endif

CFLAGS += -mno-red-zone
CFLAGS += -fPIC
CFLAGS += -undefined dynamic_lookup
CFLAGS += -I include

#-- warnings

CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wno-unused-command-line-argument

#--

default: lib-c.so

SRCS = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
OBJS = $(SRCS:.c=.o)

lib-c.so: $(OBJS)
	$(CC) $(SO_FLAGES) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(PROG) *.out *.so

lint: $(SRCS) $(HEADERS)
	clang-format -i $^
