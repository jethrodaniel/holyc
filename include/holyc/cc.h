#ifndef HOLYC_CC
#define HOLYC_CC

#include <stdbool.h>

// headers:
//   tokens
//   parser
//   codegen
//   obj
//   compiler
// src:
//   main
//     setup
//     tokens
//     parser
//     compiler

enum DebugFlags {
  DEBUG_LEX = (1 << 0),
  DEBUG_PARSE = (1 << 1),
  DEBUG_OBJ = (1 << 2),
};

typedef struct MainArgs {
  int    argc;
  char **argv;
  char **envp;
} MainArgs;

typedef int Flag;

typedef struct CompilerOpts {
  bool output_asm;
  Flag debug;
} CompilerOpts;

typedef struct Buffer {
  char *start;
  char *curr;
  int   size;
} Buffer;

typedef enum {
  // TK_ERROR,
  TK_EOF,
  TK_INT,
  TK_MIN,
  TK_PLUS,
  TK_DIV,
  TK_MUL,
  TK_LPAREN,
  TK_RPAREN,
  TK_SEMI,
} TokenType;

typedef struct Token {
  TokenType type;
  char     *start;
  int       size;
  int       value;
  int       line;
  int       col;
} Token;

typedef struct Parser {
  Token current;
  Token previous;
  int   line;
  int   col;
} Parser;

// Global compiler state.
//
typedef struct CC {
  CompilerOpts *opts;
  MainArgs      main_args;
  Buffer        input;
  Buffer        code;
  Parser        parser;
  char         *token_table[][2];
} CC;

void debug(CC *cc);

void error(char *fmt, ...);

CC *cc_init(int argc, char **argv, char **envp, int input_size);

#endif // HOLYC_CC
