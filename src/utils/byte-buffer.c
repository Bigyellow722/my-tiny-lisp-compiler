#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "byte-buffer.h"

#ifdef MODULE_TAG
#undef MODULE_TAG
#endif
#define MODULE_TAG "byte_buffer"


#define SCALE_FACTOR 2

struct bbuf_header {
  size_t used_size;
  size_t caps;
};

char* bbuf_make(size_t caps)
{
  struct bbuf_header* h = malloc(ALIGN_UP(sizeof(struct bbuf_header) + caps, 4));
  if (!h) {
    fprintf(stderr, "%s: no enough memory, err: %s\n", MODULE_TAG, strerror(errno));
    return (char*)NULL + sizeof(struct bbuf_header);
  }
  h->used_size = 0;
  h->caps = caps;
  memset((void *)((char*)h + sizeof(struct bbuf_header)), 0, caps);
  return (char*)h + sizeof(struct bbuf_header);
}

int bbuf_is_invalid(char* bbuf)
{
  struct bbuf_header* h = BBUF_TO_HEADER(bbuf);
  return h == NULL;
}

void bbuf_reset(char* bbuf)
{
  struct bbuf_header* h = BBUF_TO_HEADER(bbuf);
  memset((void*)bbuf, 0, h->used_size);
  h->used_size = 0;
}

void bbuf_free(char** bbuf)
{
  struct bbuf_header* h = BBUF_TO_HEADER(*bbuf);
  if (bbuf_is_invalid(*bbuf)) {
    free(h);
    h = NULL;
  }
}

void bbuf_dump(char* bbuf)
{
  struct bbuf_header* h = BBUF_TO_HEADER(bbuf);
  if (h) {
    fprintf(stdout, "used_size: %lu\n", BBUF_USED_SIZE(bbuf));
    fprintf(stdout, "caps: %lu\n", BBUF_CAPS(bbuf));
    fprintf(stdout, "context begin:\n");
    for (size_t i = 0; i < BBUF_USED_SIZE(bbuf); i++) {
      fprintf(stdout, "%c", bbuf[i]);
    }
    fprintf(stdout, "context end\n");
  }
}

char* _bbuf_append_raw(char* bbuf, char* src_buf, size_t buf_size)
{
  struct bbuf_header* h = NULL;
  char* tmp_bbuf = bbuf;
  size_t rest_size = BBUF_CAPS(bbuf) - BBUF_USED_SIZE(bbuf);
  if (buf_size > rest_size) {
    size_t new_caps = buf_size > BBUF_CAPS(bbuf) * SCALE_FACTOR - BBUF_USED_SIZE(bbuf) ? ALIGN_UP(buf_size + BBUF_USED_SIZE(bbuf), 4) * SCALE_FACTOR : BBUF_CAPS(bbuf) * SCALE_FACTOR;
    tmp_bbuf = bbuf_make(new_caps);
    if (bbuf_is_invalid(tmp_bbuf)) {
      fprintf(stderr, "%s, no enough mem, new_caps:(%lu) is needed\n", __func__, new_caps);
      exit(1);
    }
    memcpy(tmp_bbuf, bbuf, BBUF_USED_SIZE(bbuf));
    h = BBUF_TO_HEADER(tmp_bbuf);
    h->used_size = BBUF_USED_SIZE(bbuf);
    bbuf_free(&bbuf);
  }

  h = BBUF_TO_HEADER(tmp_bbuf);
  memcpy(tmp_bbuf + BBUF_USED_SIZE(tmp_bbuf), src_buf, buf_size);
  h->used_size += buf_size;

  return tmp_bbuf;
}


/*
 * input:
 *   src_bbuf: the byte_buffer whose context will be appended to dst byte_buffer
 *   dst_bbuf:
 * return:
 *   dst_bbuf: the dst byte_buffer after appending src_bbuf
 */
char* _bbuf_append(char* src_bbuf, char* dst_bbuf)
{
  return _bbuf_append_raw(dst_bbuf, src_bbuf, BBUF_USED_SIZE(src_bbuf));
}
