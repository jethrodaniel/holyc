#include <lib/stddef.h>
#include <lib/stdio.c>
#include <lib/stdlib.c>
#include "lib/string.c"

#include <lib/elf.h>

#define ELF_START 0x401000 // linux start address for x86_64
#define ELF_SIZE  120      // minimal elf header size

#include <lib/macho.h>

// Output a minimal x86_64 elf header to stdout.
//
int write_macho_header(int program_length) {
  warn("Writing macho header...\n");

  mach_header_64 *h = malloc(sizeof(mach_header_64));
  h->magic = MH_MAGIC_64;
  h->cputype = CPU_TYPE_X86_64;
  h->cpusubtype = CPU_SUBTYPE_X86_64_ALL;
  h->filetype = MH_EXECUTE;
  // h->ncmds =
  // h->sizeofcmds =
  // h->flags =
  // h->reserved =

  uint64_t *output = malloc(sizeof(mach_header_64));
  memcpy(output, h, sizeof(mach_header_64));

  write(STDOUT_FILENO, output, sizeof(mach_header_64));
}
