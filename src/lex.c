#include <holyc/lex.h>

#include <stdio.h>
#include <stdlib.h>

void lex_error(char *fmt, ...) {
  __asm__("call _warnf");
  exit(1);
}

Lexer *lex_new(Lexer *lex, char *input, int size) {
  lex->line        = 1;
  lex->col         = 1;
  lex->input.start = input;
  lex->input.curr  = input;
  lex->input.size  = size;

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

  warnf("[%d:%d-%d][%s, ", tok.line, tok.col, tok.col + tok.size, tokname);

  if (tok.type == TK_INT)
    warnf("'%d'", tok.value);
  else
    warnf("'%s'", lex->token_table[tok.type][1]);

  warnf("]\n", tokname);
}

bool lex_is_eof(Lexer *lex) {
  return lex->current.type == TK_EOF;
}

Token lex_make_token(Lexer *lex, TokenType type, char *start, int size,
                     TokenValue value) {
  lex->current.type  = type;
  lex->current.start = start;
  lex->current.line  = lex->line;
  lex->current.col   = lex->col;
  lex->current.value = value;
  lex->current.size  = size;

  lex->col += size;
  lex->input.curr = start + size;

  return lex->current;
}

Token lex_next_token(Lexer *lex) {
  char *c     = lex->input.curr;
  char *start = c;
  int   n;

  while (true) {
    switch (*c) {
    case '\n':
      c++;
      lex->line++;
      lex->col = 1;
      break;
    case ' ':
      c++;
      lex->col++;
      break;
    case '\0':
      return lex_make_token(lex, TK_EOF, c, 1, 0);
    case ';':
      return lex_make_token(lex, TK_SEMI, c, 1, 0);
    case '+':
      return lex_make_token(lex, TK_PLUS, c, 1, 0);
    case '-':
      return lex_make_token(lex, TK_MIN, c, 1, 0);
    case '*':
      return lex_make_token(lex, TK_MUL, c, 1, 0);
    case '/':
      return lex_make_token(lex, TK_DIV, c, 1, 0);
    case '(':
      return lex_make_token(lex, TK_LPAREN, c, 1, 0);
    case ')':
      return lex_make_token(lex, TK_RPAREN, c, 1, 0);
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
      n = 0;

      do {
        n = n * 10 + *c - '0';
        c++;
      } while (*c >= '0' && *c <= '9');

      return lex_make_token(lex, TK_INT, start, c - start, n);
    default:
      lex_error("-- error: unexpected character '%c' (%d) at column %d\n", *c,
                *c, lex->col); // todo: col, lin
    }
  }
  // if (cc->opts->debug & DEBUG_LEX)
  //   print_token(cc);
  lex->current.size = c - lex->current.start;

  return lex->current;
}

void lex_backup(Lexer *lex) {
  // if (cc->opts->debug & DEBUG_LEX)
  //   warnf("[lexer] lex_backup()\n");
  lex->input.curr = lex->current.start;
  lex->current.col -= lex->current.size;
}
