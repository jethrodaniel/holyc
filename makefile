PROG = holyc

default: clean $(PROG) run
disasm: a.out
	dd skip=120 bs=1 if=./$< 2> /dev/null | ndisasm -b64 -
run: a.out
	./$< ; echo $$?
a.out: $(PROG)
	perl -e 'print "\xB8<\x00\x00\x00\xBF*\x00\x00\x00\x0F\x05\n"' | ./$(PROG) > $@ && chmod u+x $@
test: force
	./test
force:
clean:
	rm -vf a.out $(PROG) *.o main
$(PROG): hc.c
	gcc $< -o $@
