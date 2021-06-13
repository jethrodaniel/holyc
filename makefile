PROG = holyc

default: clean $(PROG) run
run: a.out
	./$< ; echo $$?
a.out: $(PROG)
	echo hi | ./$(PROG) > $@ && chmod u+x $@
test: force
	./test
force:
clean:
	rm -vf a.out $(PROG) *.o main
$(PROG): hc.c
	gcc $< -o $@
