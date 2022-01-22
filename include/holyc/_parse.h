#ifndef HOLYC__PARSE
#define HOLYC__PARSE

#include <holyc/ast.h>
#include <holyc/lex.h>

typedef struct Parser {
  NodeNameTable node_name_table;
  Lexer         lexer;
} Parser;

Parser  *parse_new(Parser *parser, char *input, int size);
AstNode *parse_parse(Parser *parser);
void     parse_print_node(Parser *parser, AstNode *node);

#endif // HOLYC__PARSE
