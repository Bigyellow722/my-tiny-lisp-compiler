#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdio.h>
#include "token.h"
#include "utils/dynamic-array.h"

typedef struct {
  /* source file */
  FILE* src;
  char* line_buf;
  size_t line_caps;
  /* token array */
  token* token_da;
} lexer;

lexer* lexer_make(const char* file_name);
void lexer_free(lexer** l);

#endif
