// #include <holyc/cc.h>
#include <holyc/_parse.h>
#include <holyc/codegen.h>
#include <holyc/parse.h>

// bool consume(CC *cc, TokenType tokens[]) {
//   Token tok = lex_next_token(cc->parser.lexer);

//   for (TokenType t = tokens[0]; true; t++)
//     if (tok.type == t)
//       return true;

//   error("expected a '%s', got '%s' at column %d\n",
//         cc->lex->token_table[tok.type][1],
//         cc->token_table[cc->parser.lexer.current.type][1],
//         cc->parser.lexer.current.start - cc->input.start);
// }

void expect(CC *cc, TokenType t) {
  Token tok = lex_next_token(&cc->parser.lexer);

  if (tok.type != t)
    error("expected a '%s', got '%s' at column %d\n",
          cc->parser.lexer.token_table[t][1],
          cc->parser.lexer.token_table[tok.type][1], tok.col);
}

// root -> expr ';'
//       | 'if' '(' expr ')'
//
void _root(CC *cc) {
  if (cc->opts->debug & DEBUG_PARSE)
    warnf("[parser] %s()\n", __func__);

  emit_main_label(cc);

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

  Token tok;

  _term(cc, prec);

  while (true) {
    tok = lex_next_token(&cc->parser.lexer);

    if (tok.type == TK_EOF)
      return;
    if (tok.type != TK_MIN && tok.type != TK_PLUS)
      return lex_backup(&cc->parser.lexer);
    if (prec <= PREC_ADD)
      return lex_backup(&cc->parser.lexer);

    _expr(cc, PREC_ADD);
    emit_pop_rdi(cc);
    emit_pop_rax(cc);

    if (tok.type == TK_MIN)
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

  _factor(cc, prec);

  // if (cc->parser.current.type != TK_MUL) {
  //   print_token(cc);
  //   printf("shit\n");
  //   lex_backup(cc);
  //   return;
  // }

  // expect(cc, TK_MUL);

  while (true) {
    Token tok = lex_next_token(&cc->parser.lexer);
    // if (cc->parser.current.type == TK_EOF)
    //   return;
    if (tok.type != TK_MUL && tok.type != TK_DIV)
      return lex_backup(&cc->parser.lexer);

    // // if (tok != TK_MUL || tok != TK_DIV)
    // TokenType tokens[] = {TK_MUL, TK_DIV};
    // consume(cc, tokens);

    if (prec <= PREC_MUL)
      return lex_backup(&cc->parser.lexer);

    _factor(cc, PREC_MUL);
    emit_pop_rdi(cc);
    emit_pop_rax(cc);

    if (tok.type == TK_MUL)
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

  Token tok = lex_next_token(&cc->parser.lexer);

  if (tok.type != TK_INT && tok.type != TK_LPAREN)
    return lex_backup(&cc->parser.lexer);
  if (tok.type == TK_INT)
    return emit_push(cc, tok.value);

  _expr(cc, PREC_PAREN);
  expect(cc, TK_RPAREN);
}
