#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdio.h>
#include "token.h"
#include "utils/dynamic-array.h"

typedef struct {
  /* token pointer array */
  token** token_da;
} lexer;

lexer* lexer_make();
void lexer_free(lexer** l);
int lexer_tokenize_line(lexer* l, char* code, ssize_t line_sz, int line_num);
void lexer_dump_tokens(lexer* l);

#endif
