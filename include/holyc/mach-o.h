#ifndef HOLYC_SRC_MACHO
#define HOLYC_SRC_MACHO

#include <lib/mach-o/loader.h>

#include <holyc/cc.h>
#include <holyc/obj.h>

// Output a x86_64 mach-o file to stdout.
//
void write_macho(CC *cc);

#endif // HOLYC_SRC_MACHO
