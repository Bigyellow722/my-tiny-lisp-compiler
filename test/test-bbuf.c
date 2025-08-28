#include "../src/utils/byte-buffer.h"

int main()
{
  char* buf = bbuf_make(10);
  bbuf_append_raw(buf, "hello,world!\n", 14);
  bbuf_dump(buf);
  char* buf2 = bbuf_make(10);
  bbuf_append(buf2, buf);
  bbuf_dump(buf2);
}
