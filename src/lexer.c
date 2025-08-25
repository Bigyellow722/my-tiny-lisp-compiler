#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lexer.h"

lexer* lexer_make(const char* file_name)
{
  lexer* new = NULL;
  new = malloc(sizeof(lexer));
  if (!new) {
    fprintf(stderr, "no enough memory, error msg:%s\n", strerror(errno));
  }
  new->src = fopen(file_name, "r");
  if (!new->src) {
    fprintf(stderr, "failed to open %s, error msg:%s\n", file_name, strerror(errno));
    free(new);
    return NULL;
  }
  new->line_buf = NULL;
  new->line_caps = 0;
  new->token_da = darray_create(token);
  return new;
}

void lexer_free(lexer** l)
{
  if (*l) {
    fclose((*l)->src);
    darray_destroy((*l)->token_da);
    free(*l);
    *l = NULL;
  }
}
