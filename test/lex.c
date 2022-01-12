#include <holyc/lex.h>

int main(int argc, char **argv) {
  Lexer *lex = malloc(sizeof(Lexer));
  lex->token_table = {
      {"EOF",    "\\0"},
      {"INT",    ""   },
      {"MIN",    "-"  },
      {"PLUS",   "+"  },
      {"DIV",    "/"  },
      {"MUL",    "*"  },
      {"LPAREN", "("  },
      {"RPAREN", ")"  },
      {"SEMI",   ";"  },
  };

  lex->line = 1;
  lex->col = 1;
  lex->input.start = "123;";

  Token tok = lex_next_token(lex);
  (void)tok;

  lex_print_token(lex, tok);
}
