# holyc

HolyC for x86_64 Linux and Mac.

**note**: def a WIP, see [the roadmap](#roadmap).

## build

```
make
```

## what it do

- free-standing libc implementation using syscalls
- outputs program executable as ELF or Mach-O

## roadmap

One day we'll get past step I.

I: Ahead of time compiler for HolyC, written in C, outputs an executable binary.

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

- [Linux syscalls](https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/)
- [TempleOS](https://github.com/cia-foundation/TempleOS)
  - [MiniCompiler](https://github.com/cia-foundation/TempleOS/blob/archive/Demo/Lectures/MiniCompiler.HC)
- [Writing a Scheme compiler](http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf)
- [Writing a C compiler](https://www.sigbus.info/compilerbook)
  - [english translation](https://translate.google.com/translate?hl=en&sl=ja&tl=en&u=https%3A%2F%2Fwww.sigbus.info%2Fcompilerbook)
  - [that project's successor](https://github.com/rui314/chibicc)
- [Writing elf output](https://github.com/lazear/lass/blob/66771edd7fa883e0620b3e00777320e6577f7f33/assembler.c#L53)
- Program startup/boot:
  - [Linux program startup](https://web.archive.org/web/20191210114310/http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html)
  - [MacOS libc startup](https://embeddedartistry.com/blog/2019/05/20/exploring-startup-implementations-os-x/)
- Assembly hello worlds
  - https://gist.github.com/FiloSottile/7125822
- [Nascent C compilers from Bell Labs](https://github.com/dspinellis/unix-history-repo/blob/Research-V2-Snapshot-Development/c/nc0/c00.c)
- [x86_64 varargs](https://blog.nelhage.com/2010/10/amd64-and-va_arg/)
- [SysV ABI links](https://wiki.osdev.org/System_V_ABI)
- [SysV linking](https://www.cl.cam.ac.uk/~pes20/rems/papers/oopsla-elf-linking-2016.pdf)
- Syscall info
  - [MacOS](https://opensource.apple.com/source/xnu/xnu-1504.3.12/bsd/kern/syscalls.master)
- mach-o
  - https://lowlevelbits.org/parsing-mach-o-files/
