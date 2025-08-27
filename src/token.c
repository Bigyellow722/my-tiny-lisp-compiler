#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "token.h"

const char* type_strs[TOKEN_EOF + 1] = {
  "Left Parentheses",
  "Right Parentheses",
  "Symbol",
  "Integer",
  "String"
};


token* token_make(token_type type, char* buf, size_t size, int line)
{
  token* new = malloc(sizeof(token));
  if (!new) {
    return NULL;
  }
  new->type = type;
  new->line = line;
  new->size = size;
  new->value = malloc(size);
  if (!new->value) {
    free(new);
    return NULL;
  }
  memcpy(new->value, buf, size);
  return new;
}

void token_free(token** t)
{
  if (*t) {
    if ((*t)->value) {
      free((*t)->value);
      (*t)->value = NULL;
    }
    free(*t);
    *t = NULL;
  }
}

void token_dump(token* t)
{
  if (t) {
    printf("type:\t%s\n", type_strs[t->type]);
    printf("value:\t%.*s\n", t->size, t->value);
    printf("line:\t%d\n", t->line);
  }
}
