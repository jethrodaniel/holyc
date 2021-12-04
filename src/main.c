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
// Syscalls use:
//
//  rdi
//  rsi
//  rdx
//  r10
//  r9
//
//  and clobbers rcx, r11, and rax

#include "lib/crt0.c"
#include "lib/mman.c"
#include "lib/stdbool.h"
#include "lib/stddef.h"
#include "lib/stdint.h"
#include "lib/stdio.c"
#include "lib/stdlib.c"
#include "lib/string.c"
#include "lib/unistd.c"

#include "src/elf.c"
#include "src/macho.c"

// Print to stderr and exit.
//
void error(char *fmt, ...) {
  __asm__("call _warnf");
  exit(1);
}

// Global compiler state.
//
typedef struct CC {
  // globals from main()
  int    argc;
  char **argv;
  char **envp;
  bool   output_asm; // output asm, or binary?
  char  *input_buf;  // input source buffer
  char  *input;      // curr position in source buffer
  int    input_size; // length of input
  char  *code_buf;   // output code buffer
  char  *code;       // curr position in code buffer
  char  *token_pos;  // token start index
  int    token;      // token type
  int    int_val;    // if token is int

  char *token_table[8][4]; // token names, must match tokens exactly
} CC;

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

//
// Codegen
//

void emit_mov_rax_imm(CC *cc, uint64_t imm) {
  if (cc->output_asm)
    return printf("MOV RAX,%d\n", imm);

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xB8; // MOV RAX,imm
  for (int i = 0; i < 8; i++)
    *cc->code++ = imm >> 8 * i;
}

void emit_sub_rax_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("SUB RAX,RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x29; // SUB RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_add_rax_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("ADD RAX,RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x01; // ADD RAX,reg
  *cc->code++ = 0xF8; //  RDI
}

void emit_imul_rax_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("IMUL RAX,RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x0F; // IMUL RAX,reg
  *cc->code++ = 0xAF;
  *cc->code++ = 0xC7;
}

void emit_cqo_idiv_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("CQO\nIDIV RDI\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x99; //  CQO

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0xF7; //  IDIV
  *cc->code++ = 0xFF; //  RDI
}

void emit_push(CC *cc, int n) {
  if (cc->output_asm)
    return printf("PUSH %d\n", n);

  *cc->code++ = 0x68; // PUSH
  *cc->code = n;
  cc->code += 4;
}

void emit_push_rax(CC *cc) {
  if (cc->output_asm)
    return printf("PUSH RAX\n");

  *cc->code++ = 0x50; // PUSH RAX
}

void emit_pop_rax(CC *cc) {
  if (cc->output_asm)
    return printf("POP RAX\n");

  *cc->code++ = 0x58; // POP RAX
}

void emit_pop_rdi(CC *cc) {
  if (cc->output_asm)
    return printf("POP RDI\n");

  *cc->code++ = 0x5F; // POP RDI
}

void emit_syscall(CC *cc) {
  if (cc->output_asm)
    return printf("  SYSCALL");

  *cc->code++ = 0x0F; // SYSCALL
  *cc->code++ = 0x05;
}

void emit_start(CC *cc) {
  if (cc->output_asm)
    return printf("\n_start:\n"
                  "  // CALL main\n"
                  "  MOV RDI,RAX  // arg1, main()\n"
                  "  MOV RAX,60   // exit\n"
                  "  SYSCALL      // exit()\n");

  *cc->code++ = 0x48; // REX
  *cc->code++ = 0x89; // MOV RDI,reg
  *cc->code++ = 0xC7; //   RAX

#ifdef __APPLE__
  emit_mov_rax_imm(cc, 0x2000001);
#else
  emit_mov_rax_imm(cc, 60);
#endif
  emit_syscall(cc);
}

//
// Tokenizing
//

typedef enum {
  TK_EOF,
  TK_INT,
  TK_MIN,
  TK_PLUS,
  TK_DIV,
  TK_MUL,
  TK_LPAREN,
  TK_RPAREN,
} TokenType;

// Print a token for debugging.
//
void print_token(CC *cc) {
  char *tokname = cc->token_table[cc->token][0];

  if (!tokname)
    error("%s: not sure how to print token %d\n", __func__, cc->token);

  warnf("[%s, ", tokname);

  if (cc->token == TK_INT)
    warnf("'%d'", cc->int_val);
  else
    warnf("'%s'", cc->token_table[cc->token][1]);

  warnf("]\n", tokname);
}

void debug(CC *cc) {
  warnf("\n=== cc ===\n");
  warnf("\tinput_buf: %s (%d)\n", cc->input_buf, cc->input_buf);
  warnf("\tinput_size: %d\n", cc->input_size);
  warnf("\tcode: %d\n", cc->code);
  warnf("\ttoken: %d | ", cc->token);
  print_token(cc);
  warnf("==========\n");
}

// Fetches next token.
//
int Lex(CC *cc) {
  char *c = cc->input;
  int   n;

  while (true) {
    switch (*c) {
    case '\n':
    case ' ':
      c++;
      break;
    case '\0':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_EOF;
      goto ret;
    case '+':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_PLUS;
      goto ret;
    case '-':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_MIN;
      goto ret;
    case '*':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_MUL;
      goto ret;
    case '/':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_DIV;
      goto ret;
    case '(':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_LPAREN;
      goto ret;
    case ')':
      cc->token_pos = c;
      cc->input = ++c;
      cc->token = TK_RPAREN;
      goto ret;
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
      cc->token = TK_INT;
      goto ret;
    default:
      error("unexpected character '%c' (%d) at column %d\n", *c, *c,
            cc->input - cc->input_buf);
    }
  }

ret:
  print_token(cc);
  return cc->token;
}

// Unfetches next token. HACKY
//
void Unlex(CC *cc) {
  cc->input = cc->token_pos;
}

//
// Parsing
//

// Operator precedence
//
typedef enum {
  PREC_MUL,
  PREC_ADD,
  PREC_PAREN,
  PREC_TOP,
} Prec;

// Get next token, error if it's not what we expect.
//
void expect(CC *cc, TokenType t) {
  Lex(cc);
  if (cc->token != t)
    error("expected a '%s', got '%s' at column %d\n", cc->token_table[t][1],
          cc->token_table[cc->token][1], cc->input - cc->input_buf);
}

// == Grammar
//
// root -> expr EOF
// expr -> term '+' expr
//       | term '-' expr
//       | term
// term -> factor '*' factor
//       | factor '/' factor
//       | factor
// factor -> num
//         #| var
//         #| '(' expr ')'
// int -> 0..9+
// #var -> [a-zA-Z_]\w+

void _expr(CC *cc, Prec prec);
void _factor(CC *cc, Prec prec);
void _term(CC *cc, Prec prec);

// root -> expr
//
void _root(CC *cc) {
  _expr(cc, PREC_TOP);
  if (cc->token != TK_EOF)
    error("unexpected character '%s' at column %d\n",
          cc->token_table[cc->token][1], cc->input - cc->input_buf);
}

// expr -> term '+' expr
//       | term '-' expr
//       | term
//
void _expr(CC *cc, Prec prec) {
  warnf("%s: %d\n", __func__, prec);

  int tok;

  _term(cc, prec);

  while (true) {
    Lex(cc);
    tok = cc->token;

    if (cc->token == TK_EOF)
      return;
    if (tok != TK_MIN && tok != TK_PLUS)
      return Unlex(cc);
    if (prec <= PREC_ADD)
      return Unlex(cc);

    _expr(cc, PREC_ADD);
    emit_pop_rdi(cc);
    emit_pop_rax(cc);

    if (tok == TK_MIN)
      emit_sub_rax_rdi(cc);
    else
      emit_add_rax_rdi(cc);

    emit_push_rax(cc);
  }
}

// term -> factor '*' factor
//       | factor '/' factor
//       | factor
//
void _term(CC *cc, Prec prec) {
  warnf("%s: %d\n", __func__, prec);

  int tok;

  _factor(cc, prec);

  while (true) {
    Lex(cc);
    tok = cc->token;

    if (cc->token == TK_EOF)
      return;
    if (tok != TK_MUL && tok != TK_DIV)
      return Unlex(cc);
    if (prec <= PREC_MUL)
      return Unlex(cc);

    _expr(cc, PREC_MUL);
    emit_pop_rdi(cc);
    emit_pop_rax(cc);

    if (tok == TK_MUL)
      emit_imul_rax_rdi(cc);
    else
      emit_cqo_idiv_rdi(cc);

    emit_push_rax(cc);
  }
}

// factor -> num
//         | '(' expr ')'
//         #| var
//
void _factor(CC *cc, Prec prec) {
  warnf("%s: %d\n", __func__, prec);

  Lex(cc);
  int tok = cc->token;

  if (tok != TK_INT && tok != TK_LPAREN)
    return Unlex(cc);
  if (tok == TK_INT)
    return emit_push(cc, cc->int_val);

  _expr(cc, PREC_PAREN);
  expect(cc, TK_RPAREN);
}

//
// main()
//

#define INPUT_SIZE 4096

// Read code from stdin, options from argv, output asm or binary to stdout.
//
int main(int argc, char **argv, char **envp) {
  char input[INPUT_SIZE];
  int  num_read;

  CC *cc = malloc(sizeof(CC));
  cc->argc = argc;
  cc->argv = argv;
  cc->envp = envp;
  cc->input_buf = malloc(sizeof(char) * INPUT_SIZE);
  cc->input = cc->input_buf;
  cc->code_buf = malloc(sizeof(char) * INPUT_SIZE);
  cc->code = cc->code_buf;
  parse_options(cc);

  cc->token_table[0][0] = "EOF";
  cc->token_table[0][1] = "\\0";
  cc->token_table[1][0] = "INT";
  cc->token_table[1][1] = "";
  cc->token_table[2][0] = "MIN";
  cc->token_table[2][1] = "-";
  cc->token_table[3][0] = "PLUS";
  cc->token_table[3][1] = "+";
  cc->token_table[4][0] = "DIV";
  cc->token_table[4][1] = "/";
  cc->token_table[5][0] = "MUL";
  cc->token_table[5][1] = "*";
  cc->token_table[6][0] = "LPAREN";
  cc->token_table[6][1] = "(";
  cc->token_table[7][0] = "RPAREN";
  cc->token_table[7][1] = ")";

  if ((cc->input_size = read(STDIN_FILENO, cc->input, INPUT_SIZE)) < 0)
    die("read");

  warnf("read %d bytes\n", cc->input_size);

  _root(cc);

  if (cc->output_asm)
    return EXIT_SUCCESS;

  emit_pop_rax(cc);
  emit_start(cc);

  int code_size = cc->code - cc->code_buf;

  warnf("Writing %d bytes of machine code\n", code_size);

#ifdef __APPLE__
  write_macho(cc->code_buf, code_size);
#else
  write_elf(cc->code_buf, code_size);
#endif

  return EXIT_SUCCESS;
}
