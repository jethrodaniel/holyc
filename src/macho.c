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
  c->vmaddr = 0x100000000;
  c->vmsize = 0x1000;
  c->fileoff = 0;
  c->filesize = 4096;
  c->maxprot = VM_PROT_READ | VM_PROT_EXECUTE;
  c->initprot = VM_PROT_READ | VM_PROT_EXECUTE;
  c->nsects = 1;
  c->flags = 0;

  section_64 *text = malloc(sizeof(section_64));
  memcpy(text->sectname, SECT_TEXT, strlen(SECT_TEXT));
  memcpy(text->segname, SEG_TEXT, strlen(SEG_TEXT));
  text->addr = 0x100000000 + 4000;
  text->size = 83; // TODO actual code size
  text->offset = 4000;
  text->align = 1<<4;
  text->reloff = 0;
  text->nreloc = 0;
  text->flags = S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;

  // https://stackoverflow.com/a/55959072
  segment_command_64 *linkedit = malloc(sizeof(segment_command_64));
  linkedit->cmd = LC_SEGMENT_64;
  linkedit->cmdsize = 72;
  memcpy(linkedit->segname, SEG_LINKEDIT, strlen(SEG_LINKEDIT));
  linkedit->vmaddr = 0x1000;
  linkedit->vmsize = 0x1000;
  linkedit->fileoff = 4096;
  linkedit->filesize = 88;
  linkedit->maxprot = VM_PROT_READ;
  linkedit->initprot = VM_PROT_READ;
  linkedit->nsects = 0;
  linkedit->flags = 0;

  int64_t size =
      sizeof(mach_header_64) +
    + sizeof(segment_command_64) * 3
    + sizeof(section_64);

  warnf("size: %d",size);

  write(STDOUT_FILENO, h, sizeof(mach_header_64));
  write(STDOUT_FILENO, p0, sizeof(segment_command_64));
  write(STDOUT_FILENO, c, sizeof(segment_command_64));
  write(STDOUT_FILENO, text, sizeof(section_64));
  write(STDOUT_FILENO, linkedit, sizeof(segment_command_64));
}
