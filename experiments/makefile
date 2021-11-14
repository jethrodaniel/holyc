LDFLAGS := -L$(shell xcode-select -p)/SDKs/MacOSX.sdk/usr/lib -lSystem
LDFLAGS += -arch x86_64

default: hello clean

hello: hello.s
	nasm -f macho64 $<
	ld $(LDFLAGS) -o $@ $@.o
	./$@

clean:
	rm *.o hello
