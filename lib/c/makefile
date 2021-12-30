UNAME := $(shell uname)

#--

CFLAGS += -static
CFLAGS += -fPIC
CFLAGS += -nostdlib -ffreestanding
CFLAGS += -I include

CFLAGS += -g -O0

CFLAGS += -Wall
CFLAGS += -Werror

ifeq ($(UNAME), Linux)
  CFLAGS += -fleading-underscore
endif
ifeq ($(UNAME), Darwin)
  CFLAGS += -fno-stack-protector
endif

#--

SHARED_LIB := libc.a
HEADERS    := $(wildcard include/*.h)
SRCS       := $(wildcard src/*.c)
SRC_OBJS   := $(SRCS:.c=.o)
TESTS      := $(wildcard test/*.c)
TEST_BINS  := $(patsubst test/%.c,test/bin/%,$(TESTS))

default: $(SHARED_LIB)

test/bin:
	mkdir -p test/bin

test/bin/%: test/%.c $(SHARED_LIB) test/bin
	$(CC) $(CFLAGS) -L. -lc -e _start -o $@ $<

test: $(TEST_BINS)
	./test/bin/crt0
	./test/bin/stdio; code=$$?; [ $$code -eq 42 ] || echo "error -- should exit 42, got $$code";
	# ./test/bin/unistd

$(SHARED_LIB): $(SRC_OBJS)
	ar rcs $@ $^
	file $@

clean:
	rm -rf $(SRC_OBJS) $(TEST_BINS) $(SHARED_LIB) test/bin

lint: $(SRCS) $(HEADERS) $(TESTS)
	clang-format -i $^
