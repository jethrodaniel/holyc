// = holyc
//
// SysV x86_64 fn call registers (in order), rest on the stack
//
// - rdi
// - rsi
// - rdx
// - rcx
// - r8
// - r9
//
// Linux syscalls use:
//
//  rdi
//  rsi
//  rdx
//  r10
//  r9
//
//  clobbers rcx, r11, and rax


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

typedef struct CC {
  int argc;
  char **argv;
  char **envp;
  bool output_asm; // output asm, or binary?
  char *input_buf; // input source buffer
  char *input;     // curr position in source buffer
  int input_size;  // length of input
  char *code_buf;  // output code buffer
  char *code;      // curr position in code buffer
  char *token_pos; // token start index
  int token;       // token type
  int int_val;     // if token is int
} CC;

void error(char *fmt, ...) {
  asm("call warnf");
  exit(1);
}

// Set defaults, update cc options from argv values.
//
void parse_options(CC *cc) {
  for (int i = 0; i < cc->argc; i++) {
    char *arg = cc->argv[i];

    if (*arg == '-')
      arg++;
    else
      continue;

    if (*arg == 'S')
      cc->output_asm = true;
  }
}

void emit_mov_rax_imm(CC *cc, int imm) {
  if (cc->output_asm) {
    printf("MOV RAX,%d\n", imm);
    return;
  }
  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xB8; // MOV RAX,imm
  *cc->code = imm;
  cc->code += 8;
}

void emit_sub_rax_rdi(CC *cc) {
  if (cc->output_asm) {
    printf("SUB RAX,RDI\n");
    return;
  }
  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x29; // SUB RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_add_rax_rdi(CC *cc) {
  if (cc->output_asm) {
    printf("ADD RAX,RDI\n");
    return;
  }
  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x01; // ADD RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_imul_rax_rdi(CC *cc) {
  if (cc->output_asm) {
    printf("IMUL RAX,RDI\n");
    return;
  }
  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x0F; // IMUL RAX,reg
  *cc->code++ = 0xAF;
  *cc->code++ = 0xC7;
}

void emit_cqo_idiv_rdi(CC *cc) {
  if (cc->output_asm) {
    printf("CQO\n");
    printf("IDIV RDI\n");
    return;
  }
  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x99; //  CQO

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xF7; //  IDIV
  *cc->code++ = 0xFF; //  RDI
}

void emit_push(CC *cc, int n) {
  if (cc->output_asm) {
    printf("PUSH %d\n", n);
    return;
  }
  *cc->code++ = 0x68; // PUSH
  *cc->code = n;
  cc->code += 4;
}

void emit_push_rax(CC *cc) {
  if (cc->output_asm) {
    printf("PUSH RAX\n");
    return;
  }
  *cc->code++ = 0x50; // PUSH RAX
}


void emit_pop_rax(CC *cc) {
  if (cc->output_asm) {
    printf("POP RAX\n");
    return;
  }
  *cc->code++ = 0x58; // POP RAX
}

void emit_pop_rdi(CC *cc) {
  if (cc->output_asm) {
    printf("POP RDI\n");
    return;
  }
  *cc->code++ = 0x5F; // POP RDI
}

void emit_syscall(CC *cc) {
  if (cc->output_asm) {
    printf("  SYSCALL");
    return;
  }
  *cc->code++ = 0x0F; // SYSCALL
  *cc->code++ = 0x05;
}

void emit_start(CC *cc) {
  if (cc->output_asm) {
    printf("\n_start:\n");
    printf("  // CALL main\n");
    printf("  MOV RDI,RAX  // arg1, main()\n");
    printf("  MOV RAX,60   // exit\n");
    printf("  SYSCALL      // exit()\n");
    return;
  }

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x89; // MOV RDI,reg
  *cc->code++ = 0xC7; //   RAX

  emit_mov_rax_imm(cc, 60);
  emit_syscall(cc);
}

typedef enum {
  TK_EOF,
  TK_INT,
  TK_MIN,
  TK_PLUS,
  TK_DIV,
  TK_MUL,
} TokenType;

typedef enum {
  PREC_MUL,
  PREC_ADD,
  PREC_TOP,
} Prec;

void print_token(CC *cc) {
  switch (cc->token) {
    case TK_EOF:
      warnf("[EOF, '\\0']\n", cc->token);
      break;
    case TK_INT:
      warnf("[INT, '%d']\n", cc->int_val);
      break;
    case TK_MIN:
      warnf("[MIN, '-']\n");
      break;
    case TK_PLUS:
      warnf("[PLUS, '+']\n");
      break;
  }
}

// Fetches next token.
//
int Lex(CC *cc) {
  char *c = cc->input;
  int n;

  while (true) {
    switch (*c) {
      case '\n':
      case ' ':
        c++;
        break;
      case '\0':
        cc->input = ++c;
        return cc->token = TK_EOF;
      case '+':
        cc->token_pos = c;
        cc->input = ++c;
        return cc->token = TK_PLUS;
      case '-':
        cc->token_pos = c;
        cc->input = ++c;
        return cc->token = TK_MIN;
      case '*':
        cc->token_pos = c;
        cc->input = ++c;
        return cc->token = TK_MUL;
      case '/':
        cc->token_pos = c;
        cc->input = ++c;
        return cc->token = TK_DIV;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        cc->token_pos = c;

        n = 0;

        do {
          n = n * 10 + *c - '0';
          c++;
        } while (*c >= '0' && *c <= '9');

        cc->int_val = n;
        cc->input = c;
        return cc->token = TK_INT;
      default:
        error("unexpected character '%c' (%d)", *c, *c);
    }
  }
}

void expect(CC *cc, TokenType t) {
  Lex(cc);
  if (cc->token != t)
    error("expected a TOKEN=%d, got '%c' (%d)", t, cc->token, cc->token);
}

// == Grammar
//
// root -> expr
// expr -> term '+' expr
//       | term '-' expr
//       | term
// term -> factor '*' factor
//       | factor '/' factor
//       | factor
// factor -> num
//         | var
//         | '(' expr ')'
// int -> 0..9+
// var -> [a-zA-Z_]\w+

void _expr(CC *cc, Prec prec);
void _factor(CC *cc, Prec prec);
void _term(CC *cc, Prec prec);

// root -> expr
//
void _root(CC *cc) {
  _expr(cc, PREC_TOP);
  if (cc->token != TK_EOF)
    error("unexpected character '%d' (ascii) | token: %d\n", *cc->input, cc->token);
}

// expr -> term '+' expr
//       | term '-' expr
//       | term
//
void _expr(CC *cc, Prec prec) {
  int tok;

  _term(cc, prec);
   print_token(cc);

  while (true) {
    Lex(cc);
    tok = cc->token;

    if (cc->token == TK_EOF) return;

    if (tok != TK_MIN && tok != TK_PLUS) {
      cc->input = cc->token_pos; // unlex
      return;
    }

    if (prec <= PREC_ADD) {
      cc->input = cc->token_pos; // unlex
      return;
    }

    _expr(cc, PREC_ADD);
    emit_pop_rdi(cc);
    emit_pop_rax(cc);

    if (tok == TK_MIN) emit_sub_rax_rdi(cc);
    else               emit_add_rax_rdi(cc);

    emit_push_rax(cc);
  }
}

// term -> factor '*' factor
//       | factor '/' factor
//       | factor
//
void _term(CC *cc, Prec prec) {
  int tok;

  _factor(cc, prec);

  while (true) {
    Lex(cc);
    tok = cc->token;

    if (cc->token == TK_EOF) return;

    if (tok != TK_MUL && tok != TK_DIV) {
      cc->input = cc->token_pos; // unlex
      return;
    }

    if (prec <= PREC_MUL) {
       cc->input = cc->token_pos; // unlex
       return;
     }

     _expr(cc, PREC_MUL);
     emit_pop_rdi(cc);
     emit_pop_rax(cc);

     if (tok == TK_MUL) emit_imul_rax_rdi(cc);
     else               emit_cqo_idiv_rdi(cc);

     emit_push_rax(cc);
  }
}

// factor -> num
//         #| var
//         #| '(' expr ')'
//
void _factor(CC *cc, Prec prec) {
  expect(cc, TK_INT);
  print_token(cc);
  emit_push(cc, cc->int_val);
}


#define INPUT_SIZE 4096

int main(int argc, char **argv, char **envp) {
  char input[INPUT_SIZE];
  int num_read;

  CC *cc = malloc(sizeof(CC));
  cc->argc = argc;
  cc->argv = argv;
  cc->envp = envp;
  cc->input_buf = malloc(sizeof(char) * INPUT_SIZE);
  cc->input = cc->input_buf;
  cc->code_buf= malloc(sizeof(char) * INPUT_SIZE);
  cc->code = cc->code_buf;
  parse_options(cc);

  if ((cc->input_size = read(STDIN_FILENO, cc->input, INPUT_SIZE)) < 0)
    die("read");

  _root(cc);
  emit_pop_rax(cc);
  emit_start(cc);

  int code_size = cc->code - cc->code_buf;

  if (!cc->output_asm)
    write_elf_header(code_size);

  warnf("Writing %d bytes of machine code\n", code_size);
  write(STDOUT_FILENO, cc->code_buf, code_size);

  return EXIT_SUCCESS;
}
