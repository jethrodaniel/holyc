#ifndef HOLYC_SRC_PARSE
#define HOLYC_SRC_PARSE

#include "lib/stdio.c"

#include "src/cc.c"
#include "src/codegen.c"
#include "src/lex.c"

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

void _expr(CC *cc, Prec prec);
void _factor(CC *cc, Prec prec);
void _term(CC *cc, Prec prec);

// root -> expr
//
void _root(CC *cc) {
  if (cc->opts->debug_parser)
    warnf("%s\n", __func__);

  emit_main_label(cc);
  _expr(cc, PREC_TOP);
  expect(cc, TK_EOF);
}

// expr -> term '+' expr
//       | term '-' expr
//       | term
//
void _expr(CC *cc, Prec prec) {
  if (cc->opts->debug_parser)
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
  if (cc->opts->debug_parser)
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
  if (cc->opts->debug_parser)
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

#endif // HOLYC_SRC_PARSE
