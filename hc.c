// = holyc
//
// == references
//
// - Terry Davis's mini compiler demo:
//   https://github.com/cia-foundation/TempleOS/blob/archive/Demo/Lectures/MiniCompiler.HC
//
// - Michael Lazear's assembler:
//   https://github.com/lazear/lass/blob/master/assembler.c#L53
//
// - Elf quine: https://medium.com/@MrJamesFisher/understanding-the-elf-4bd60daac571
//   https://medium.com/@MrJamesFisher/understanding-the-elf-4bd60daac571

// SysV x86_64 fn call registers (in order), rest on the stack
//
// - rdi
// - rsi
// - rdx
// - rcx
// - r8
// - r9

// Linux syscalls use:
//
//  %rdi, %rsi, %rdx, %r10, %r8 and %r9
//
//  clobbers %rcx, %r11, and %rax


#include "src/boot.c"
#include "src/stddef.h"
#include "src/stdint.h"
#include "src/stdlib.c"
#include "src/unistd.c"
#include "src/mman.c"
#include "src/string.c"
#include "src/stdio.c"

// stupid malloc
//
// https://my.eng.utah.edu/~cs4400/malloc.pdf
//
void *malloc(int n) {
  return mmap(
    NULL, // let kernel decide where the mem is
    n,
    PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_ANONYMOUS | MAP_PRIVATE,
    -1, // map anon
    0   // no offset
  );
}

#define INPUT_SIZE 4096

#include "src/elf.h"

// #define ELF_START 0x8048000
#define ELF_START 0x401000
#define ELF_SIZE  120

int write_elf(int program_length) {
  uint64_t elf_offset = 0;
  uint8_t *elf_output;

  warn("Writing elf header...\n");

  elf_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);

  printf("elf_offset: %d\n", elf_offset);
  printf("Elf64_Ehdr: %d\n", sizeof(Elf64_Ehdr));

  // Elf64_Ehdr *e = malloc(sizeof(Elf64_Ehdr));
  // e->e_ident[EI_MAG0]       = ELFMAG0;
  // e->e_ident[EI_MAG1]       = ELFMAG1;
  // e->e_ident[EI_MAG2]       = ELFMAG2;
  // e->e_ident[EI_MAG3]       = ELFMAG3;
  // e->e_ident[EI_CLASS]      = ELFCLASS64;
  // e->e_ident[EI_DATA]       = ELFDATA2LSB;
  // e->e_ident[EI_VERSION]    = EV_CURRENT;
  // e->e_ident[EI_OSABI]      = ELFOSABI_SYSV;
  // e->e_ident[EI_ABIVERSION] = 0;
  // e->e_ident[EI_PAD]        = 0;
  // e->e_type                 = ET_EXEC;
  // e->e_machine              = EM_X86_64;
  // e->e_version              = EV_CURRENT;
  // e->e_entry                = ELF_START + ELF_SIZE;
  // e->e_phoff                = sizeof(Elf64_Ehdr);
  // e->e_shoff                = 0;
  // e->e_flags                = 0;
  // e->e_ehsize               = sizeof(Elf64_Ehdr);
  // e->e_phentsize            = sizeof(Elf64_Phdr);
  // e->e_phnum                = 1;
  // e->e_shentsize            = 0;
  // e->e_shnum                = 0;
  // e->e_shstrndx             = 0;
}

//   Elf64_Phdr *p = malloc(sizeof(Elf64_Phdr));
//   p->p_type   = 1;
//   p->p_flags  = 5;
//   p->p_offset = 0;
//   p->p_vaddr  = ELF_START;
//   p->p_paddr  = 0x8048000;
//   p->p_filesz = program_length + elf_offset;
//   p->p_memsz = p->p_filesz;
//   p->p_align = 0x1000;
//   info("p_filesz: %ld bytes\n", p->p_filesz);

//   elf_output = malloc(elf_offset);
//   memcpy(elf_output, e, sizeof(Elf64_Ehdr));
//   memcpy(elf_output + sizeof(Elf64_Ehdr), p, sizeof(Elf64_Phdr));

//   write(STDOUT_FILENO, elf_output, elf_offset);
// }

int main(int argc, char **argv, char **envp) {
  char input[INPUT_SIZE];
  int num_read;

  // if ((num_read = read(STDIN_FILENO, input, INPUT_SIZE)) < 0)
  //   die("read");

  // printf("read %d bytes\n", num_read);

  // write_elf(num_read);
  // write(STDOUT_FILENO, &input, num_read - 1); // rm \n

  unsigned char code [] = {
    0x55,                   // push rbp
    0x48, 0x89, 0xe5,       // mov  rbp, rsp
    0x89, 0x7d, 0xfc,       // mov  DWORD PTR [rbp-0x4],edi
    0x89, 0x75, 0xf8,       // mov  DWORD PTR [rbp-0x8],esi
    0x8b, 0x75, 0xfc,       // mov  esi,DWORD PTR [rbp-04x]
    0x0f, 0xaf, 0x75, 0xf8, // imul esi,DWORD PTR [rbp-0x8]
    0x89, 0xf0,             // mov  eax,esi
    0x5d,                   // pop  rbp
    0xc3                    // ret
  };

  void* vmem = mmap(
    NULL, // let kernel decide where the mem is
    sizeof(code),
    PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_ANONYMOUS | MAP_PRIVATE,
    -1, // map anon
    0   // no offset
  );

  memcpy(vmem, code, sizeof(code));
  int (*func)() = vmem;
  printf("%d * %d = %d\n", 5, 11, func(5, 11));

  char *mem = vmem;
  mem[0] = 'A';
  mem[1] = 'B';
  printf("mem[0]=%c\n", mem[0]);

  munmap(vmem, sizeof(code));

  return EXIT_SUCCESS;
}
