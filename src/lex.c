#include <holyc/lex.h>

#include <stdio.h>
#include <stdlib.h>

void lex_error(char *fmt, ...) {
  __asm__("call _warnf");
  exit(1);
}

Lexer *lex_new(char *input, int size) {
  Lexer *lex = malloc(sizeof(Lexer));

  lex->line = 1;
  lex->col = 1;
  lex->input.start = input;
  lex->input.curr = input;
  lex->input.size = size;

  TokenTable token_table = {
      {"UNINITIALIZED", "???"},
      {"EOF",           "\\0"},
      {"INT",           ""   },
      {"MIN",           "-"  },
      {"PLUS",          "+"  },
      {"DIV",           "/"  },
      {"MUL",           "*"  },
      {"LPAREN",        "("  },
      {"RPAREN",        ")"  },
      {"SEMI",          ";"  },
  };

  for (int i = 0; i < (sizeof(token_table) / 16); i++) {
    lex->token_table[i][0] = token_table[i][0];
    lex->token_table[i][1] = token_table[i][1];
  }

  return lex;
}

void lex_print_token(Lexer *lex, Token tok) {
  char *tokname = lex->token_table[tok.type][0];

  if (!tokname)
    lex_error("%s: not sure how to print token %d\n", __func__, tok.type);

  warnf("[%d:%d][%s, ", tok.line + 1, tok.col + 1, tokname);

  if (tok.type == TK_INT)
    warnf("'%d'", tok.value);
  else
    warnf("'%s'", lex->token_table[tok.type][1]);

  warnf("]\n", tokname);
}

bool lex_is_eof(Lexer *lex) {
  return lex->current.type == TK_EOF;
}

// char *lex_next_char(Lexer *lex) {
//   char *c = lex.input.curr++;

//   if (c == '\n') {
//     lex->line++;
//     lex->col = 1;
//   } else
//     lex->col++;

//   return c;
// }

Token lex_next_token(Lexer *lex) {
  // if (lex_is_eof(lex))
  // return lex->current;

  char *c = lex->input.curr;
  int   n;

  while (true) {
    // printf("c: '%s'\n", c);
    switch (*c) {
    case '\n':
      // lex->
    case ' ':
      c++;
      break;
    case '\0':
      lex->current.start = c;
      lex->current.type = TK_EOF;
      lex->input.curr = ++c;
      goto ret;
    case ';':
      lex->current.start = c;
      lex->current.type = TK_SEMI;
      lex->input.curr = ++c;
      goto ret;
    // case '+':
    //   cc->parser.current.start = c;
    //   cc->input.curr = ++c;
    //   cc->parser.current.type = TK_PLUS;
    //   goto ret;
    // case '-':
    //   cc->parser.current.start = c;
    //   cc->input.curr = ++c;
    //   cc->parser.current.type = TK_MIN;
    //   goto ret;
    // case '*':
    //   cc->parser.current.start = c;
    //   cc->input.curr = ++c;
    //   cc->parser.current.type = TK_MUL;
    //   goto ret;
    // case '/':
    //   cc->parser.current.start = c;
    //   cc->input.curr = ++c;
    //   cc->parser.current.type = TK_DIV;
    //   goto ret;
    // case '(':
    //   cc->parser.current.start = c;
    //   cc->input.curr = ++c;
    //   cc->parser.current.type = TK_LPAREN;
    //   goto ret;
    // case ')':
    //   cc->parser.current.start = c;
    //   cc->input.curr = ++c;
    //   cc->parser.current.type = TK_RPAREN;
    //   goto ret;
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
      lex->current.start = c;

      n = 0;

      do {
        n = n * 10 + *c - '0';
        c++;
      } while (*c >= '0' && *c <= '9');

      lex->current.value = n;
      lex->current.type = TK_INT;
      lex->input.curr = c;
      goto ret;
    default:
      lex_error("-- error: unexpected character '%c' (%d) at column %d\n", *c,
                *c, lex->col); // todo: col, lin
    }
  }
ret:
  // if (cc->opts->debug & DEBUG_LEX)
  //   print_token(cc);
  lex->current.size = c - lex->current.start;

  return lex->current;
}

//// Unfetches next token. HACKY
////
// void Unlex(CC *cc) {
//   if (cc->opts->debug & DEBUG_LEX)
//     warnf("[lexer] Unlex()\n");
//   cc->input.curr = cc->parser.current.start;
// }
