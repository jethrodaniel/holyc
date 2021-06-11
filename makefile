default: clean holyc
test: force
	./test
force:
clean:
	rm -f a.out holyc
holyc: hc.c
	gcc $< -o $@
