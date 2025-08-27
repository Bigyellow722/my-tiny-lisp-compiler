#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stddef.h>
  
typedef enum {
  TOKEN_LPAREN = 0,
  TOKEN_RPAREN,
  TOKEN_SYMBOL,
  TOKEN_INTEGER,
  TOKEN_STRING,
  TOKEN_EOF
} token_type;


typedef struct {
  token_type type;
  int line;
  int size;
  char* value;
} token;

token* token_make(token_type type, char* buf, size_t size, int line);
void token_free(token** t);
void token_dump(token* t);
#endif
