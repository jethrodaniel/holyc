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
//  rdi
//  rsi
//  rdx
//  r10
//  r9
//
//  clobbers %rcx, %r11, and %rax


#include "lib/boot.c"
#include "lib/stddef.h"
#include "lib/stdint.h"
#include "lib/mman.c"
#include "lib/stdlib.c"
#include "lib/unistd.c"
#include "lib/string.c"
#include "lib/stdio.c"
#include "lib/elf.h"
#include "lib/stdbool.h"

#define ELF_START 0x401000
#define ELF_SIZE  120

int write_elf_header(int program_length) {
  uint64_t elf_offset = 0;
  uint8_t *elf_output;

  warn("Writing elf header...\n");

  elf_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);

  Elf64_Ehdr *e = malloc(sizeof(Elf64_Ehdr));
  e->e_ident[EI_MAG0]       = ELFMAG0;
  e->e_ident[EI_MAG1]       = ELFMAG1;
  e->e_ident[EI_MAG2]       = ELFMAG2;
  e->e_ident[EI_MAG3]       = ELFMAG3;
  e->e_ident[EI_CLASS]      = ELFCLASS64;
  e->e_ident[EI_DATA]       = ELFDATA2LSB;
  e->e_ident[EI_VERSION]    = EV_CURRENT;
  e->e_ident[EI_OSABI]      = ELFOSABI_SYSV;
  e->e_ident[EI_ABIVERSION] = 0;
  e->e_ident[EI_PAD]        = 0;
  e->e_type                 = ET_EXEC;
  e->e_machine              = EM_X86_64;
  e->e_version              = EV_CURRENT;
  e->e_entry                = ELF_START + ELF_SIZE;
  e->e_phoff                = sizeof(Elf64_Ehdr);
  e->e_shoff                = 0;
  e->e_flags                = 0;
  e->e_ehsize               = sizeof(Elf64_Ehdr);
  e->e_phentsize            = sizeof(Elf64_Phdr);
  e->e_phnum                = 1;
  e->e_shentsize            = 0;
  e->e_shnum                = 0;
  e->e_shstrndx             = 0;

  Elf64_Phdr *p = malloc(sizeof(Elf64_Phdr));
  p->p_type   = 1;
  p->p_flags  = 5;
  p->p_offset = 0;
  p->p_vaddr  = ELF_START;
  p->p_paddr  = ELF_START;
  p->p_filesz = program_length + elf_offset;
  p->p_memsz = p->p_filesz;
  p->p_align = 0x1000; // ?

  elf_output = malloc(elf_offset);
  memcpy(elf_output, e, sizeof(Elf64_Ehdr));
  memcpy(elf_output + sizeof(Elf64_Ehdr), p, sizeof(Elf64_Phdr));

  write(STDOUT_FILENO, elf_output, elf_offset);
}

typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenType;

typedef struct Token Token;
struct Token {
  TokenType type;
  Token *next;
  int val;
  char *str;
};

// expr -> term '*' term
//       | term '/' term
// term -> num '+' num
//       | num '-' num
// num -> '-' _num
//      | _num
// _num -> digits
//       | digits '.' digits
//       | 'E' digits
// digit  -> 0..9
// digits -> digit+
// digit  -> 0..9


// types:
//
// struct Token

struct Lex {
  char *src;
  int curr;
  int pos;
};

typedef enum {
  OPT_OUTPUT_ASM,
  OPT_OUTPUT_BIN,
} OptionOutputFormat;

typedef struct Options Options;

struct Options {
  OptionOutputFormat output_format;
};

bool output_bin(Options *opts) {
  return opts->output_format == OPT_OUTPUT_BIN;
}
bool output_asm(Options *opts) {
  return opts->output_format == OPT_OUTPUT_ASM;
}

void parse_options(Options *opts, int argc, char **argv) {
  // defaults
  opts->output_format = OPT_OUTPUT_BIN;

  for (int i = 0; i < argc; i++) {
    char *arg = argv[i];

    if (*arg == '-')
      arg++;
    else
      continue;

    if (*arg == 'S')
      opts->output_format = OPT_OUTPUT_ASM;
  }
}

#define INPUT_SIZE 4096

void output_start() {
}

// process input:
//   lex
//   parse
//   generate
//   (jit) execute


int process(char *input, int size, Options *opts) {
  char code[INPUT_SIZE];
  char *c = code;
  char *p = input;

  if (output_bin(opts))
    write_elf_header(size);

  if (output_bin(opts)) {
    write_elf_header(size);
    *c++ = 0x48; // REX
    *c++ = 0xB8; // MOV RAX,imm
    *c = strtol(p, &p, 10);
    c += 8;
  } else {
    printf("\n// _main::\n");
    printf("    MOV      RAX,%d\n", strtol(p, &p, 10));
  }

  if (output_bin(opts)) {
    // _start:
    //     call  main
    //     mov   rdi,rax
    //     mov   rax,60   ; exit
    //     syscall
    //
    *c++ = 0x48; // REX
    *c++ = 0x89; // MOV RDI,reg
    *c++ = 0xC7; //   RAX

    *c++ = 0x48; // REX
    *c++ = 0xB8; // MOV RAX,imm
    *c = 0x3c;   //   60 (exit)
    c += 8;

    *c++ = 0x0F; // SYSCALL
    *c++ = 0x05;

    warnf("Writing %d bytes of machine code\n", c - code);
    write(STDOUT_FILENO, code, c - code);
  } else if (output_asm(opts)) {
    printf("\n// _start:\n");
    printf("    // CALL  _main\n");
    printf("    MOV       RDI,RAX  // arg1, _main()\n");
    printf("    MOV       RAX,60   // exit()\n");
    printf("    SYSCALL   \n");
  }

  int n = 0;

  // while (*p) {
  //   switch (*p) {
  //     case ' ':
  //     case '\t':
  //     case '\n':
  //       warnf("skipping space: %c (%d)\n", *p, *p);
  //       break;
  //     case '+':
  //       *c++ = 0x48; // REX
  //       *c++ = 0x05; // ADD RAX,imm
  //       break;
  //     case '-':
  //       *c++ = 0x48; // REX
  //       *c++ = 0x2D; // SUB RAX,imm
  //       break;
  //     case '0':
  //     case '1':
  //     case '2':
  //     case '3':
  //     case '4':
  //     case '5':
  //     case '6':
  //     case '7':
  //     case '8':
  //     case '9':
  //       n = 0;

  //       do {
  //         n = n * 10 + *p++ - '0';
  //       } while (*p >= '0' && *p <= '9');

  //       *((uint32_t *)c) = n;
  //       c += sizeof(uint32_t);
  //       warnf("n: %d\n", n);

  //       break;
  //     default:
  //       warnf("unexpected character: %c (%d)\n", *p, *p);
  //       exit(2);
  //   }
  //   *p++;
  // }

  // ...

  return EXIT_SUCCESS;
}

int main(int argc, char **argv, char **envp) {
  char input[INPUT_SIZE];
  int num_read;

  Options *opts = malloc(sizeof(Options));
  parse_options(opts, argc, argv);

  if ((num_read = read(STDIN_FILENO, input, INPUT_SIZE)) < 0)
    die("read");

  // lex
  // preprocess
  // parse
  // type-check
  // resolve
  return process(input, num_read, opts);
}


