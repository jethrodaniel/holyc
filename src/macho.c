#include <lib/stddef.h>
#include <lib/stdio.c>
#include <lib/stdlib.c>
#include "lib/string.c"

#include <lib/elf.h>

#define ELF_START 0x401000 // linux start address for x86_64
#define ELF_SIZE  120      // minimal elf header size

#include <lib/macho.h>
//https://medium.com/tokopedia-engineering/a-curious-case-of-mach-o-executable-26d5ecadd995

// Output a minimal x86_64 mach-o file to stdout.
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
  h->flags = MH_NOUNDEFS;
  // h->reserved =

  segment_command_64 *p0 = malloc(sizeof(segment_command_64));
  p0->cmd = LC_SEGMENT_64;
  p0->cmdsize = 72;
  memcpy(p0->segname, SEG_PAGEZERO, strlen(SEG_PAGEZERO));
  p0->vmsize = 0x100000000;

  segment_command_64 *c = malloc(sizeof(segment_command_64));
  c->cmd = LC_SEGMENT_64;
  c->cmdsize = 72;
  memcpy(c->segname, SEG_TEXT, strlen(SEG_TEXT));
  c->maxprot = VM_PROT_READ | VM_PROT_EXECUTE;
  c->initprot = VM_PROT_READ | VM_PROT_EXECUTE;

  section_64 *s = malloc(sizeof(section_64));
  memcpy(s->sectname, SEC_TEXT, strlen(SEC_TEXT));
  memcpy(s->segname, SEG_TEXT, strlen(SEG_TEXT));
  // s->maxprot = VM_PROT_READ | VM_PROT_EXECUTE;
  // s->initprot = VM_PROT_READ | VM_PROT_EXECUTE;

  int64_t size =
      sizeof(mach_header_64) +
    + sizeof(segment_command_64) * 2
    + sizeof(section_64);

  warnf("size: %d",size);

  write(STDOUT_FILENO, h, sizeof(mach_header_64));
  write(STDOUT_FILENO, p0, sizeof(segment_command_64));
  write(STDOUT_FILENO, c, sizeof(segment_command_64));
  write(STDOUT_FILENO, s, sizeof(section_64));
}
