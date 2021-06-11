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
#include <stdarg.h>
#include <errno.h>
#include <elf.h>

#define INPUT_SIZE 4096

void info(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[holyc]: ");
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void die(char *msg) {
  perror(msg);
}

int main(int, char **);

int write_elf(int program_length)
{
  uint64_t elf_offset = 0;
  uint8_t *elf_output;

  info("Making elf header...\n");

  elf_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);
  Elf64_Ehdr *e = malloc(sizeof(Elf64_Ehdr));
  e->e_ident[0]  = 0x7f; // magic
  e->e_ident[1]  = 'E';
  e->e_ident[2]  = 'L';
  e->e_ident[3]  = 'F';
  e->e_type      = ET_EXEC;    // 2
  e->e_machine   = EM_X86_64;  // 62
  e->e_version   = EV_CURRENT; // 1
  e->e_entry     = (Elf64_Addr)main;
  e->e_phoff     = sizeof(Elf64_Ehdr);
  e->e_shoff     = 0;
  e->e_flags     = 0;
  e->e_ehsize    = sizeof(Elf64_Ehdr);
  e->e_phentsize = sizeof(Elf64_Phdr);
  e->e_phnum     = 1;
  e->e_shentsize = 0;
  e->e_shnum     = 0;
  e->e_shstrndx  = 0;

  Elf64_Phdr *p = malloc(sizeof(Elf64_Phdr));
  p->p_type   = 1;
  p->p_flags  = 5;
  p->p_offset = 0;
  p->p_vaddr  = 0;
  p->p_paddr  = 0;
  p->p_filesz = program_length + elf_offset;
  info("p_filesz: %ld bytes\n", p->p_filesz);
  p->p_memsz = p->p_filesz;
  p->p_align = 0x1000;

  elf_output = malloc(elf_offset);
  memcpy(elf_output, e, sizeof(Elf64_Ehdr));
  memcpy(elf_output + sizeof(Elf64_Ehdr), p, sizeof(Elf64_Phdr));

  // write(STDOUT_FILENO, &elf_output, elf_output +);
  fwrite(elf_output, sizeof(char), elf_offset, stdout);

  // int code_size
  // uint8_t *code = malloc(elf_offset);
  // fwrite(code, sizeof(char), code_size, stdout);
  // fwrite(code, sizeof(char), current_position, fd);
}



int main(int argc, char **argv)
{
  uint8_t input[INPUT_SIZE];
  int num_read;

  while ((num_read = read(STDIN_FILENO, input, INPUT_SIZE)) > 0) {
    info("read %d bytes\n", num_read);
  }

  if (num_read < 0) {
    die("read");
    return errno;
  }

  write_elf(num_read);

  return EXIT_SUCCESS;
}
