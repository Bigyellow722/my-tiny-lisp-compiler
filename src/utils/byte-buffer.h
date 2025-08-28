#ifndef _BTYE_BUFFER_H_
#define _BTYE_BUFFER_H_

#include <stddef.h>

struct bbuf_header;

#define BBUF_TO_HEADER(BBUF) ((struct bbuf_header*)(BBUF - sizeof(struct bbuf_header)))
#define HEADER_TO_BBUF(H) ((char*)H + sizeof(struct bbuf_header))
#define BBUF_USED_SIZE(BBUF) (BBUF_TO_HEADER(BBUF)->used_size)
#define BBUF_CAPS(BBUF) (BBUF_TO_HEADER(BBUF)->caps)

char* bbuf_make(size_t caps);
void bbuf_reset(char* bbuf);
void bbuf_free(char** bbuf);
void bbuf_dump(char* bbuf);
char* _bbuf_append_raw(char* bbuf, char* src_buf, size_t buf_size);
char* _bbuf_append(char* src_bbuf, char* dst_bbuf);

#define bbuf_append_raw(bbuf, src, size) \
  { \
    bbuf = _bbuf_append_raw(bbuf, src, size); \
  }

#define bbuf_append(dest, src) \
  { \
    dest = _bbuf_append(dest, src); \
  }
#endif
