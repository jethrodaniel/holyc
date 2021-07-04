# holyc

HolyC for Linux, x86_64.

**note**: def a WIP, see [the roadmap](#roadmap).


## build

```
make
```

## roadmap

One day we'll get past step I.

I: Ahead of time compiler for HolyC, written in C, outputs ELF binary.

    cc holyc.c -o hc0
    ./hc0 <test.hc >test

II: Bootstrap AOT in HolyC

    ./hc0 <hc.hc >hc1 && chmod u+x hc1
    ./hc1 <test.hc >test

III: Add JIT support

    hc1 <jit.hc <main.hc >holyc

After:

    holyc --aot <jit.hc <main.hc >holyc
    holyc --jit

IV: Write basic shell

    holyc --jit <shell.hc

## references

- [TempleOS](https://github.com/cia-foundation/TempleOS)
- [Writing a Scheme compiler](http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf)
- [Writing a C compiler](https://www.sigbus.info/compilerbook)
  - [english translation](https://translate.google.com/translate?hl=en&sl=ja&tl=en&u=https%3A%2F%2Fwww.sigbus.info%2Fcompilerbook)
  - [that project's successor](https://github.com/rui314/chibicc)
- [Writing elf output](https://github.com/lazear/lass/blob/66771edd7fa883e0620b3e00777320e6577f7f33/assembler.c#L53)
- [Linux program startup](https://web.archive.org/web/20191210114310/http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html)
- [Nascent C compilers from Bell Labs](https://github.com/dspinellis/unix-history-repo/blob/Research-V2-Snapshot-Development/c/nc0/c00.c)
- [x86_64 varargs](https://blog.nelhage.com/2010/10/amd64-and-va_arg/)
