# holyc

HolyC for Linux, x86_64.

## build

```
make
```

## roadmap

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
