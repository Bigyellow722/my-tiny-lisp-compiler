#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lexer.h"

lexer* lexer_make()
{
  lexer* new = NULL;
  new = malloc(sizeof(lexer));
  if (!new) {
    fprintf(stderr, "no enough memory, error msg:%s\n", strerror(errno));
  }
  new->token_da = darray_create(token*);
  return new;
}

void lexer_free(lexer** l)
{
  if (*l) {
    for(size_t i = 0; i < DARRAY_COUNT((*l)->token_da); i++) {
      token_free(&(*l)->token_da[i]);
    }
    darray_destroy(&(*l)->token_da);
    free(*l);
    *l = NULL;
  }
}

void lexer_dump_tokens(lexer* l)
{
  for(size_t i = 0; i < DARRAY_COUNT(l->token_da); i++) {
    token_dump(l->token_da[i]);
  }
}

int lexer_tokenize_line(lexer* l, char* code, ssize_t line_sz, int line_num)
{
  int start = 0, cur = 0;
  token* new = NULL;
  while(cur <= line_sz) {
    char c = code[cur];
    switch (c) {
    case ' ':
    case '\t':
    case '\r':
      cur++;
      break;
    case '(':
      new = token_make(TOKEN_LPAREN, "(", 1, line_num);
      darray_push(l->token_da, new);
      cur++;
      break;
    case ')':
      new = token_make(TOKEN_RPAREN, ")", 1, line_num);
      darray_push(l->token_da, new);
      cur++;
      break;
    case '\'':
      cur++;
      break;
    default:
      cur++;
      break;
    }
  }
  return 0;
}
