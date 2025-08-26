#include "../src/utils/dynamic-array.h"
#include <stdio.h>
#include <string.h>
static int test_int()
{
  DEFINE_DARRAY(test1, int);
  if (darray_is_invalid(test1)) {
    fprintf(stderr, "failed to create test1 array\n");
    return -1;
  }
  int* test2 = darray_create(int);
  if (darray_is_invalid(test2)) {
    fprintf(stderr, "failed to create test2 array\n");
    darray_destroy((void**)&test1);
    return -1;
  }

  for (int i = 0 ; i < 100; i ++) {
    darray_push(test1, i);
    darray_push(test2, i);
  }
  for (int i = 0; i < 100; i++) {
    if (test1[i] != test2[i]) {
      fprintf(stderr, "failed to pass the test\n");
      darray_destroy(&test2);
      darray_destroy(&test1);
      return -1;
    }
  }
  for (int i = 0; i < 100; i++) {
    test1[i] = test1[i] + test2[i];
  }
  for (int i = 0; i < 100; i++) {
    if (test1[i] != 2 * test2[i]) {
      fprintf(stderr, "failed to pass the test\n");
      darray_destroy(&test2);
      darray_destroy(&test1);
      return -1;
    }
  }
  fprintf(stdout, "test-int-darray pass\n");
  darray_destroy(&test2);
  darray_destroy(&test1);
  return 0;
}

static int test_str()
{
  int ret;
  DEFINE_DARRAY(test1, char);
  if (darray_is_invalid(test1)) {
    fprintf(stderr, "failed to create test1 array\n");
    return -1;
  }
  darray_push(test1, 'h');
  darray_push(test1, 'e');
  darray_push(test1, 'l');
  darray_push(test1, 'l');
  darray_push(test1, 'o');
  darray_push(test1, ',');
  darray_push(test1, 'w');
  darray_push(test1, 'o');
  darray_push(test1, 'r');
  darray_push(test1, 'l');
  darray_push(test1, 'd');
  darray_push(test1, '\0');
  const char* res = "hello,world";
  ret = strncmp(test1, res, 12);
  if (ret) {
    darray_destroy(&test1);
    fprintf(stdout, "test-str-darray failed\n");
    return ret;
  }
  darray_destroy(&test1);
  fprintf(stdout, "test-str-darray pass\n");
  return ret;
}

int main(void)
{
  int ret = 0;
  ret = test_int();
  ret = test_str();
  return ret;
}
