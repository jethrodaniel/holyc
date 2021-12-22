#ifndef HOLYC_SRC_OBJ
#define HOLYC_SRC_OBJ

#include <holyc/cc.h>

// rdi: cc
// rsi: fmt
// rdx: varg1
// rcx: varg2
// r8:  varg3
// r9:  varg4
//
void log(CC *cc, char *fmt, ...);

#include <holyc/elf.h>
#include <holyc/mach-o.h>

void write_macho(CC *cc);
void write_elf(CC *cc);

void write_executable(CC *cc);

#endif // HOLYC_SRC_OBJ
