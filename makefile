default: clean holyc run
run: a.out
	./a.out ; echo $$?
a.out:
	nasm -f bin -o $@ elf.s && chmod +x $@
test: force
	./test
force:
clean:
	rm -vf a.out holyc *.o
holyc: hc.c
	gcc $< -o $@
