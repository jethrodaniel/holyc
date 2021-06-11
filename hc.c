// = holyc
//
// == references
//
// - Terry Davis's mini compiler demo:
//   https://github.com/cia-foundation/TempleOS/blob/archive/Demo/Lectures/MiniCompiler.HC

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>

int main(int argc, char **argv);

int write_elf(int program_length)
{
  uint64_t elf_offset = 0;
  uint8_t *elf_output;

  printf("Making elf header\n");

  elf_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);
  Elf64_Ehdr *e = malloc(sizeof(Elf64_Ehdr));
  e->e_ident[0] = 0x7f; // magic
  e->e_ident[1] = 'E';
  e->e_ident[2] = 'L';
  e->e_ident[3] = 'F';
  e->e_type = ET_EXEC;
  e->e_machine = EM_X86_64;
  e->e_version = EV_CURRENT;
  e->e_entry = (Elf64_Addr)main;
  e->e_phoff = sizeof(Elf64_Ehdr);
  e->e_shoff = 0;
  e->e_flags = 0;
  e->e_ehsize = sizeof(Elf64_Ehdr);
  e->e_phentsize = sizeof(Elf64_Phdr) * 1;
  e->e_phnum = 1;
  e->e_shentsize = 0;
  e->e_shnum = 0;
  e->e_shstrndx = 0;

  Elf64_Phdr *p = malloc(sizeof(Elf64_Phdr));
  p->p_type = 1;
  p->p_offset = 0;
  p->p_vaddr = 0;
  p->p_paddr = 0;
  p->p_filesz = program_length + elf_offset;
  printf("p_filesz: %ld bytes\n", p->p_filesz);
  p->p_memsz = p->p_filesz;
  p->p_flags = 0x5;
  p->p_align = 0x1000;

  elf_output = malloc(elf_offset);
  memcpy(elf_output, e, sizeof(Elf64_Ehdr));
  memcpy(elf_output + sizeof(Elf64_Ehdr), p, sizeof(Elf64_Phdr));

  fwrite(elf_output, 1, elf_offset, stdout);
  fflush(stdout);
}

int main(int argc, char **argv)
{
  char buf[255];
  int num_read;

  while ((num_read = read(STDIN_FILENO, buf, 255)) > 0) {
    // write(STDOUT_FILENO, &buf, num_read);
  }
  write_elf(num_read);
  printf("%c", buf[1]);

  return EXIT_SUCCESS;
}
