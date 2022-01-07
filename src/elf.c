#include <stdio.h>
#include <stdint.h>

#include <holyc/cc.h>
#include <holyc/elf.h>

void write_elf(CC *cc) {
  uint64_t elf_offset = 0;
  char    *elf_output;
  int      code_size = cc->code.curr - cc->code.start;

  warn("Writing elf header...\n");

  elf_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);

  Elf64_Ehdr *e = malloc(sizeof(Elf64_Ehdr));
  e->e_ident[EI_MAG0] = ELFMAG0;
  e->e_ident[EI_MAG1] = ELFMAG1;
  e->e_ident[EI_MAG2] = ELFMAG2;
  e->e_ident[EI_MAG3] = ELFMAG3;
  e->e_ident[EI_CLASS] = ELFCLASS64;
  e->e_ident[EI_DATA] = ELFDATA2LSB;
  e->e_ident[EI_VERSION] = EV_CURRENT;
  e->e_ident[EI_OSABI] = ELFOSABI_SYSV;
  e->e_ident[EI_ABIVERSION] = 0;
  e->e_ident[EI_PAD] = 0;
  e->e_type = ET_EXEC;
  e->e_machine = EM_X86_64;
  e->e_version = EV_CURRENT;
  e->e_entry = ELF_START + ELF_SIZE;
  e->e_phoff = sizeof(Elf64_Ehdr);
  e->e_shoff = 0;
  e->e_flags = 0;
  e->e_ehsize = sizeof(Elf64_Ehdr);
  e->e_phentsize = sizeof(Elf64_Phdr);
  e->e_phnum = 1;
  e->e_shentsize = 0;
  e->e_shnum = 0;
  e->e_shstrndx = 0;

  Elf64_Phdr *p = malloc(sizeof(Elf64_Phdr));
  p->p_type = 1;
  p->p_flags = 5;
  p->p_offset = 0;
  p->p_vaddr = ELF_START;
  p->p_paddr = ELF_START;
  p->p_filesz = code_size + elf_offset;
  p->p_memsz = p->p_filesz;
  p->p_align = 0x1000; // ?

  elf_output = malloc(elf_offset);
  memcpy(elf_output, e, sizeof(Elf64_Ehdr));
  memcpy(elf_output + sizeof(Elf64_Ehdr), p, sizeof(Elf64_Phdr));

  write(STDOUT_FILENO, elf_output, elf_offset);
  write(STDOUT_FILENO, cc->code.start, code_size);
}
