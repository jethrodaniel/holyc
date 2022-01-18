#include <holyc/obj.h>

// rdi: cc
// rsi: fmt
// rdx: varg1
// rcx: varg2
// r8:  varg3
// r9:  varg4
//
void log(CC *cc, char *fmt, ...) {
  // if (!(cc->opts->debug & DEBUG_OBJ))
  //   return;

  __asm__("mov %rsi, %rdi"); // mv fmt to arg1
  __asm__("mov %rdx, %rsi"); // mv varg1 to arg2
  __asm__("mov %rcx, %rdx"); // mv varg2 to arg3
  __asm__("mov %r8, %rcx");  // mv varg3 to arg4
  __asm__("mov %r9, %r8");   // mv varg4 to arg5
  __asm__("call _warnf");
}

void write_macho(CC *cc);
void write_elf(CC *cc);

void write_executable(CC *cc) {
#ifdef __APPLE__
  write_macho(cc);
#else
  write_elf(cc);
#endif
}
