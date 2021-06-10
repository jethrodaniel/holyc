default: clean holyc
	echo hi | ./holyc
test: force
	./test
force:
clean:
	rm -f a.out holyc
holyc: hc.c
	gcc $< -o $@
