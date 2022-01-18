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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <holyc/cc.h>
#include <holyc/codegen.h>
#include <holyc/lex.h>
#include <holyc/obj.h>
#include <holyc/parse.h>

#define INPUT_SIZE 4096

int main(int argc, char **argv, char **envp) {
  char *USAGE =
      "Usage: holyc [command]\n\n"
      "Command:\n"
      "  lex                     print tokens\n"
      "  parse                   print AST\n"
      "  cc, compile             print executable\n"
      "  help, --help, -h        show this help\n"
      "  version, --version, -v  show version\n"
      "\n"
      "Examples:\n"
      "  while true; do holyc parse; done\n"
      "  printf '1+2*(5-1)' | holyc cc > a.out && chmod u+x a.out\n\n";

  bool lex, parse, compile, help;

  if (argc == 1)
    help = true;

  for (int i = 1; i < argc; i++) {
    char *arg = argv[i];

    if (strcmp(arg, "lex") == 0)
      lex = true;
    else if (strcmp(arg, "parse") == 0)
      parse = true;
    else if (strcmp(arg, "compile") == 0 || strcmp(arg, "cc") == 0)
      compile = true;
    else if (strcmp(arg, "help") == 0 || strcmp(arg, "--help") == 0 ||
             strcmp(arg, "-h") == 0)
      help = true;
    else if (strcmp(arg, "version") == 0 || strcmp(arg, "--version") == 0 ||
             strcmp(arg, "-v") == 0) {
      printf("holyc v0.0.0\n");
      exit(0);
    } else {
      warnf("unknown command '%s'\n", arg);
      exit(1);
    }
  }

  if (help) {
    printf("%s", USAGE);
    exit(0);
  }

  CC *cc = cc_init(argc, argv, envp, INPUT_SIZE);

  if (cc_read(cc, STDIN_FILENO, INPUT_SIZE) < 0)
    die("cc_read");

  if (lex) {
    Lexer *lex = &cc->parser.lexer;

    Token tok;

    while (!lex_is_eof(lex)) {
      tok = lex_next_token(lex);
      lex_print_token(lex, tok);
    }
  } else if (parse) {
    AstNode *node = parse_parse(&cc->parser);
    parse_print_node(&cc->parser, node);
  } else if (compile) {
    _root(cc);
    emit_start(cc);
    write_executable(cc);
  }

  return EXIT_SUCCESS;
}
