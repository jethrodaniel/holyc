#include <stdio.h>

#include <holyc/mach-o.h>

void write_macho(CC *cc) {
  int code_size = cc->code.curr - cc->code.start;

  // if (cc->opts->debug & DEBUG_OBJ) {
  //   warn("Writing macho ...\n");
  //   warnf("Writing %d bytes of machine code\n", code_size);
  //   warnf("code_size: %i\n", code_size);
  // }

  mach_header_64 *h = malloc(sizeof(mach_header_64));
  h->magic          = MH_MAGIC_64;
  h->cputype        = CPU_TYPE_X86_64;
  h->cpusubtype     = CPU_SUBTYPE_X86_64_ALL;
  h->filetype       = MH_EXECUTE;
  h->ncmds          = 7;
  h->sizeofcmds     = 544;
  h->flags          = MH_NOUNDEFS;

  segment_command_64 *p0 = malloc(sizeof(segment_command_64));
  p0->cmd                = LC_SEGMENT_64;
  p0->cmdsize            = 72;
  memcpy(p0->segname, SEG_PAGEZERO, strlen(SEG_PAGEZERO));
  p0->vmsize = 0x100000000;

  segment_command_64 *c = malloc(sizeof(segment_command_64));
  c->cmd                = LC_SEGMENT_64;
  c->nsects             = 1;
  c->cmdsize = sizeof(segment_command_64) + sizeof(section_64) * c->nsects;
  memcpy(c->segname, SEG_TEXT, strlen(SEG_TEXT));
  c->vmaddr   = 0x100000000;
  c->vmsize   = 0x1000;
  c->fileoff  = 0;
  c->filesize = 4096;
  c->maxprot  = VM_PROT_READ | VM_PROT_EXECUTE;
  c->initprot = VM_PROT_READ | VM_PROT_EXECUTE;
  c->flags    = 0;

  section_64 *text = malloc(sizeof(section_64));
  memcpy(text->sectname, SECT_TEXT, strlen(SECT_TEXT));
  memcpy(text->segname, SEG_TEXT, strlen(SEG_TEXT));
  text->addr   = 0x100000000 + 4000;
  text->size   = code_size;
  text->offset = 4000;
  text->align  = 1 << 2;
  text->reloff = 0;
  text->nreloc = 0;
  text->flags  = S_ATTR_PURE_INSTRUCTIONS | S_ATTR_SOME_INSTRUCTIONS;

  // https://stackoverflow.com/a/55959072
  segment_command_64 *linkedit = malloc(sizeof(segment_command_64));
  linkedit->cmd                = LC_SEGMENT_64;
  linkedit->cmdsize            = 72;
  memcpy(linkedit->segname, SEG_LINKEDIT, strlen(SEG_LINKEDIT));
  linkedit->vmaddr   = 0x100000000 + 0x1000;
  linkedit->vmsize   = 0x1000;
  linkedit->fileoff  = 4096;
  linkedit->filesize = 88;
  linkedit->maxprot  = VM_PROT_READ;
  linkedit->initprot = VM_PROT_READ;
  linkedit->nsects   = 0;
  linkedit->flags    = 0;

  symtab_command *symtbl = malloc(sizeof(symtab_command));
  symtbl->cmd            = LC_SYMTAB;
  symtbl->cmdsize        = sizeof(symtab_command);
  symtbl->symoff         = 4096;
  symtbl->nsyms          = 3; // main,_start,exit
  symtbl->stroff         = 4096 + 40 + 8;
  symtbl->strsize        = 40;

  uuid_command *uuid = malloc(sizeof(uuid_command));
  uuid->cmd          = LC_UUID;
  uuid->cmdsize      = sizeof(uuid_command);
  uuid->uuid[0]      = (uint8_t)42;

  source_version_command *v = malloc(sizeof(source_version_command));
  v->cmd                    = LC_SOURCE_VERSION;
  v->cmdsize                = 16;
  v->version                = 0;

  thread_command *t = malloc(sizeof(thread_command));
  t->cmd            = LC_UNIXTHREAD;
  t->cmdsize = sizeof(thread_command) + sizeof(_STRUCT_X86_THREAD_STATE64);
  t->flavor  = x86_THREAD_STATE64;
  t->count   = x86_THREAD_STATE64_COUNT;

  _STRUCT_X86_THREAD_STATE64 *reg = malloc(sizeof(_STRUCT_X86_THREAD_STATE64));
  reg->rip                        = text->addr;

  int64_t size = sizeof(mach_header_64) + +sizeof(segment_command_64) * 3 +
                 sizeof(section_64) + sizeof(symtab_command) +
                 sizeof(uuid_command) + sizeof(source_version_command) +
                 sizeof(thread_command) + sizeof(_STRUCT_X86_THREAD_STATE64);

  int64_t  padsize = text->offset - size;
  int64_t *pad     = malloc(padsize);

  int64_t *code = malloc(code_size);
  memcpy(code, cc->code.start, code_size);

  int64_t actual_symtblsize = sizeof(nlist_64) * symtbl->nsyms;

  nlist_64 actual_symtbl[symtbl->nsyms];

  actual_symtbl[0].n_un.n_strx = 1;
  actual_symtbl[1].n_un.n_strx = 2;
  actual_symtbl[2].n_un.n_strx = 3;

  int64_t  bufsize = 4184 - size - padsize - code_size - actual_symtblsize;
  int64_t *buf     = malloc(bufsize);

  // uint8_t *strtbl = malloc(symtbl->strsize);
  // memcpy(strtbl, "main", 5);

  int64_t offset = 0;
  size           = 0;

  (void)offset; // TODO: why is this needed?

  // log(cc, "--> Outputing Mach-O executable\n", offset);

  // log(cc, "\tmach_header_64: %i\n", offset);
  write(STDOUT_FILENO, h, size = sizeof(mach_header_64));
  offset += size;

  // log(cc, "\tpagezero: %i\n", offset);
  write(STDOUT_FILENO, p0, size = sizeof(segment_command_64));
  offset += size;

  // log(cc, "\t__TEXT: %i\n", offset);
  write(STDOUT_FILENO, c, size = sizeof(segment_command_64));
  offset += size;

  // log(cc, "\t__text: %i\n", offset);
  write(STDOUT_FILENO, text, size = sizeof(section_64));
  offset += size;

  // log(cc, "\t__LINKEDIT: %i\n", offset);
  write(STDOUT_FILENO, linkedit, size = sizeof(segment_command_64));
  offset += size;

  // log(cc, "\tLC_SYMTAB: %i\n", offset);
  write(STDOUT_FILENO, symtbl, size = sizeof(symtab_command));
  offset += size;

  // log(cc, "\tLC_UUID: %i\n", offset);
  write(STDOUT_FILENO, uuid, size = sizeof(uuid_command));
  offset += size;

  // log(cc, "\tLC_SOURCE_VERSION: %i\n", offset);
  write(STDOUT_FILENO, v, size = sizeof(source_version_command));
  offset += size;

  // log(cc, "\tLC_UNIXTHREAD: %i\n", offset);
  write(STDOUT_FILENO, t, size = sizeof(thread_command));
  offset += size;

  // log(cc, "\tLC_UNIXTHREAD.state: %i\n", offset);
  write(STDOUT_FILENO, reg, size = sizeof(_STRUCT_X86_THREAD_STATE64));
  offset += size;

  // log(cc, "\tpadding: %i\n", offset);
  write(STDOUT_FILENO, pad, size = padsize);
  offset += size;

  // log(cc, "\ttext: %i\n", offset);
  write(STDOUT_FILENO, code, size = code_size);
  offset += size;

  // log(cc, "\tsymtbl: %i\n", offset);
  write(STDOUT_FILENO, actual_symtbl, size = actual_symtblsize);
  offset += size;

  // log(cc, "\tbuf: %i\n", offset);
  write(STDOUT_FILENO, buf, size = bufsize);
  offset += size;

  // // log(cc, "\tstrtbl: %i\n", offset);
  // write(STDOUT_FILENO, strtbl, size = symtbl->strsize);
  // offset += size;

  // log(cc, "\tfinal offset: %i\n", offset);
}
