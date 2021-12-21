# holyc

HolyC for x86_64 Linux and Mac.

**note**: def a WIP, more of a teaching project for now.

## what it do

- free-standing libc implementation using syscalls
- outputs program executables directly (ELF or Mach-O)

## philosophy

- nothing external, unless necessary (elf.h, mach-o/loader.h, etc)

## build

```
git clone https://github.com/jethrodaniel/holyc
cd holyc
make
```

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
  - https://medium.com/tokopedia-engineering/a-curious-case-of-mach-o-executable-26d5ecadd995
  - https://developer.apple.com/library/archive/documentation/Performance/Conceptual/CodeFootprint/Articles/MachOOverview.html

## license

holyc as a whole is licensed under the following standard MIT license:

```
Copyright (c) 2021 Mark Delk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

The following files have their own license information, see the file for details:

- [lib/mach-o/loader.h](lib/mach-o/loader.h) (APSL)
