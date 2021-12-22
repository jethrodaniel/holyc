#ifndef HOLYC_SRC_PARSE
#define HOLYC_SRC_PARSE

#include <holyc/cc.h>
#include <holyc/codegen.h>
#include <holyc/lex.h>

#include <stdio.h>

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
void expect(CC *cc, TokenType t);

// == Grammar
//
// root -> {expr}+ EOF
// expr -> term '+' expr
//       | term '-' expr
//       | term
// term -> factor '*' factor
//       | factor '/' factor
//       | factor
// factor -> 0..9+
//         #| var
//         #| '(' expr ')'
// #var -> [a-zA-Z_]\w+

void _root(CC *cc);
void _expr(CC *cc, Prec prec);
void _term(CC *cc, Prec prec);
void _factor(CC *cc, Prec prec);

#endif // HOLYC_SRC_PARSE
