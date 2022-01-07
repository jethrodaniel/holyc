#ifndef HOLYC_SRC_LEX
#define HOLYC_SRC_LEX

#include <holyc/cc.h>

#include <stdio.h>
#include <stdlib.h>

void error_at(CC *cc);

// Print a token for debugging.
//
void print_token(CC *cc);

// Fetches next token.
//
int Lex(CC *cc);

// Unfetches next token. HACKY
//
void Unlex(CC *cc);

#endif // HOLYC_SRC_LEX
