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
int write_macho_header(int program_length, uint8_t *code_buf) {
  warn("Writing macho header...\n");

  mach_header_64 *h = malloc(sizeof(mach_header_64));
  h->magic = MH_MAGIC_64;
  h->cputype = CPU_TYPE_X86_64;
  h->cpusubtype = CPU_SUBTYPE_X86_64_ALL;
  h->filetype = MH_EXECUTE;
  h->ncmds = 7;
  h->sizeofcmds = 544;
  h->flags = MH_NOUNDEFS;
  // h->reserved =

  segment_command_64 *p0 = malloc(sizeof(segment_command_64));
  p0->cmd = LC_SEGMENT_64;
  p0->cmdsize = 72;
  memcpy(p0->segname, SEG_PAGEZERO, strlen(SEG_PAGEZERO));
  p0->vmsize = 0x100000000;

  segment_command_64 *c = malloc(sizeof(segment_command_64));
  c->cmd = LC_SEGMENT_64;
  c->cmdsize = 152;
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
  text->size = 0x53; // TODO actual code size
  text->offset = 4000;
  text->align = 1<<2;
  text->reloff = 0;
  text->nreloc = 0;
  text->flags = S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;

  // https://stackoverflow.com/a/55959072
  segment_command_64 *linkedit = malloc(sizeof(segment_command_64));
  linkedit->cmd = LC_SEGMENT_64;
  linkedit->cmdsize = 72;
  memcpy(linkedit->segname, SEG_LINKEDIT, strlen(SEG_LINKEDIT));
  linkedit->vmaddr = 0x100000000 + 0x1000;
  linkedit->vmsize = 0x1000;
  linkedit->fileoff = 4096;
  linkedit->filesize = 88;
  linkedit->maxprot = VM_PROT_READ;
  linkedit->initprot = VM_PROT_READ;
  linkedit->nsects = 0;
  linkedit->flags = 0;

  symtab_command *symtbl = malloc(sizeof(symtab_command));
  symtbl->cmd = LC_SYMTAB;
  symtbl->cmdsize = sizeof(symtab_command);
  symtbl->symoff = 4096;
  symtbl->nsyms = 3; // main,_start,exit
  symtbl->stroff = 4096 + 40 + 8;
  symtbl->strsize = 40;

  uuid_command *uuid = malloc(sizeof(uuid_command));
  uuid->cmd = LC_UUID;
  uuid->cmdsize = sizeof(uuid_command);
  uuid->uuid[0] = (uint8_t)42;

  source_version_command *v = malloc(sizeof(source_version_command));
  v->cmd = LC_SOURCE_VERSION;
  v->cmdsize = 16;
  v->version = 0;

  thread_command *t = malloc(sizeof(thread_command));
  t->cmd = LC_UNIXTHREAD;
  t->cmdsize = sizeof(thread_command)
    + sizeof(_STRUCT_X86_THREAD_STATE64);
  t->flavor = x86_THREAD_STATE64;
  t->count = x86_THREAD_STATE64_COUNT;

  _STRUCT_X86_THREAD_STATE64 *reg = malloc(sizeof(_STRUCT_X86_THREAD_STATE64));
  reg->rip = text->addr;

 int64_t size =
      sizeof(mach_header_64) +
    + sizeof(segment_command_64) * 3
    + sizeof(section_64)
    + sizeof(symtab_command)
    + sizeof(uuid_command)
    + sizeof(source_version_command)
    + sizeof(thread_command)
    + sizeof(_STRUCT_X86_THREAD_STATE64);

  int64_t padsize = text->offset - size;
  int64_t *pad = malloc(padsize);

  int64_t codesize = symtbl->symoff - (text-> offset + text->size);
  int64_t *code = malloc(codesize);

  // int64_t padsize = symtbl->symoff - size;
  // int64_t *pad = malloc(padsize);

  int64_t actual_symtblsize = sizeof(nlist_64) * symtbl->nsyms;

  // array of nlist_64
  nlist_64 actual_symtbl[symtbl->nsyms];

  actual_symtbl[0].n_un.n_strx = 1;
  actual_symtbl[1].n_un.n_strx = 2;
  actual_symtbl[2].n_un.n_strx = 3;

  int64_t bufsize = 4184 - size - padsize - codesize - actual_symtblsize;
  int64_t *buf = malloc(bufsize);

  warnf("symtbl->symoff: %d\n",symtbl->symoff);
  warnf("text->offset: %d\n",text->offset);
  warnf("text->size: %d\n",text->size);
  warnf("padsize: %d\n",padsize);
  warnf("padsize+codesize: %d\n",padsize+codesize);
  warnf("size: %d\n",size);
  warnf("bufsize: %d\n",bufsize);
  warnf("text->offset+text->size: %d\n",text->offset+text->size);

  write(STDOUT_FILENO, h, sizeof(mach_header_64));
  write(STDOUT_FILENO, p0, sizeof(segment_command_64));
  write(STDOUT_FILENO, c, sizeof(segment_command_64));
  write(STDOUT_FILENO, text, sizeof(section_64));
  write(STDOUT_FILENO, linkedit, sizeof(segment_command_64));
  write(STDOUT_FILENO, symtbl, sizeof(symtab_command));
  write(STDOUT_FILENO, uuid, sizeof(uuid_command));
  write(STDOUT_FILENO, v, sizeof(source_version_command));
  write(STDOUT_FILENO, t, sizeof(thread_command));
  write(STDOUT_FILENO, reg, sizeof(_STRUCT_X86_THREAD_STATE64));
  write(STDOUT_FILENO, pad, padsize);
  write(STDOUT_FILENO, code, codesize);
  write(STDOUT_FILENO, actual_symtbl, actual_symtblsize);
  write(STDOUT_FILENO, buf, bufsize);
}
