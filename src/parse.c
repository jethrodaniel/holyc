#include <holyc/parse.h>

// void accept(CC *cc, TokenType t) {
//   Lex(cc);
//   if (cc->curr_token.type != t)

void expect(CC *cc, TokenType t) {
  Lex(cc);
  if (cc->curr_token.type != t)
    error("expected a '%s', got '%s' at column %d\n", cc->token_table[t][1],
          cc->token_table[cc->curr_token.type][1], cc->input.curr - cc->input.start);
}

// root -> expr ';'
//       | 'if' '(' expr ')'
//
void _root(CC *cc) {
  if (cc->opts->debug & DEBUG_PARSE)
    warnf("[parser] %s()\n", __func__);

  emit_main_label(cc);

  if (cc->curr_token.type != TK_EOF)
    return;

  _expr(cc, PREC_TOP);
  expect(cc, TK_SEMI);

  expect(cc, TK_EOF);
  emit_pop_rax(cc);
}

// expr -> term '+' expr
//       | term '-' expr
//       | term
//
void _expr(CC *cc, Prec prec) {
  if (cc->opts->debug & DEBUG_PARSE)
    warnf("[parser] %s(%d)\n", __func__, prec);

  int tok;

  _term(cc, prec);

  while (true) {
    Lex(cc);
    tok = cc->curr_token.type;

    if (cc->curr_token.type == TK_EOF)
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
  if (cc->opts->debug & DEBUG_PARSE)
    warnf("[parser] %s(%d)\n", __func__, prec);

  int tok;

  _factor(cc, prec);

  while (true) {
    Lex(cc);
    tok = cc->curr_token.type;

    if (cc->curr_token.type == TK_EOF)
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
  if (cc->opts->debug & DEBUG_PARSE)
    warnf("[parser] %s(%d)\n", __func__, prec);

  Lex(cc);
  int tok = cc->curr_token.type;

  if (tok != TK_INT && tok != TK_LPAREN)
    return Unlex(cc);
  if (tok == TK_INT)
    return emit_push(cc, cc->curr_token.value);

  _expr(cc, PREC_PAREN);
  expect(cc, TK_RPAREN);
}
