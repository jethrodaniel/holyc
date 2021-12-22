#ifndef HOLYC_SRC_ELF
#define HOLYC_SRC_ELF

#include <elf.h>
#include <holyc/cc.h>
#include <holyc/obj.h>

#define ELF_START 0x401000 // linux start address for x86_64
#define ELF_SIZE  120      // minimum elf header size

// Output a x86_64 elf file to stdout.
//
void write_elf(CC *cc);

#endif // HOLYC_SRC_ELF
