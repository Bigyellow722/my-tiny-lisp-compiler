#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dynamic-array.h"

void darray_iterate(void* array, void (*func)(void* param))
{
  size_t len = DARRAY_COUNT(array);
  size_t stride = DARRAY_STRIDE(array);
  for (size_t i = 0; i < len; i++) {
    func((void*)((char*)array + stride * i));
  }
}

/*
 * output:
 *  @array_ptr: return a pointer to the allocated array. On error, return NULL + sizeof(darray_header).
 * input:
 *  @capacity:
 *  @stride: the size of item
 */
void* _darray_create(size_t capacity, size_t stride, int factor)
{
  assert(capacity != 0 && factor != 0);
  size_t header_size = sizeof(struct dynamic_array_header);
  size_t array_size = capacity * stride;
  void* new_array = NULL;
  struct dynamic_array_header* header = NULL;

  new_array = malloc(header_size + array_size);
  if (!new_array) {
    fprintf(stderr, "%s, line(%d): failed to malloc (%lu) with caps(%lu) stride(%lu) factor(%d)\n", __func__, __LINE__, header_size + array_size, capacity, stride, factor);
    return (void*)((char*)NULL + header_size);
  }
  memset(new_array, 0, header_size + array_size);
  header = new_array;
  header->capacity = capacity;
  header->stride = stride;
  header->count = 0;
  header->factor = factor;
  return (void *)((char *)new_array + header_size);
}

int darray_is_invalid(void* array)
{
  char* ptr = (char*)DARRAY_TO_HEADER(array);
  return  ptr == NULL;
}

void _darray_destroy(void** array)
{
  if (!darray_is_invalid(*array)) {
    size_t header_size = sizeof(struct dynamic_array_header);
    struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)(*array) - header_size);
    free(header);
    header = NULL;
  }
}

/* output:
   @new array_ptr: return a pointer to the pointer to new resized array, On error, exit
 * input:
 * @array_ptr: return a pointer to the pointer to array that needs to be resized. 
 */
static void* darray_resize(void* array)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);

  void* new_array = _darray_create(header->capacity * header->factor, header->stride, header->factor);
  if (darray_is_invalid(new_array)) {
    fprintf(stderr, "%s, no enough memory\n", __func__);
    exit(1);
  }

  memmove(new_array, array, header->count * header->stride);
  
  struct dynamic_array_header* new_header = (struct dynamic_array_header*)((char *)new_array - header_size);
  new_header->count = header->count;
  _darray_destroy((void**)&array);
  return new_array;
}

/* output:
 * @new array ptr: return a pointer to the new array after append a new item pointed by @value_ptr. On error, exit
 */
void* _darray_push(void* array, const void* value_ptr)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  if (header->count >= header->capacity) {
    array = darray_resize(array);
    header = (struct dynamic_array_header*)((char *)array - header_size);
  }

  void* new_value = (void*)((char*)array + header->count * header->stride);
  memmove(new_value, value_ptr, header->stride);
  header->count++;
  return array;
}

void* _darray_insert_at(void* array, size_t index, const void* value_ptr)
{
  size_t len = DARRAY_COUNT(array);
  size_t stride = DARRAY_STRIDE(array);
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  if (header->count >= header->capacity) {
    array = darray_resize(array);
    header = (struct dynamic_array_header*)((char *)array - header_size);
  }

  void* tmp = (void*)((char*)array + index * stride);
  void* tmp_next = (void*)((char*)array + (index + 1) * stride);
  memmove(tmp_next, tmp, len - index);
  memmove(tmp, value_ptr, stride);
  header->count++;
  return array;
}

void darray_pop(void* array, void* dest)
{
  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  if (header->count < 1) {
    fprintf(stderr, "%s, the darray is empty\n", __func__);
    return;
  }

  if (dest) {
    memmove(dest, (void*)((char*)array + (header->count - 1) * header->stride), header->stride);
  }
  header->count--;
}


void* darray_pop_at(void* array, size_t index, void* dest)
{
  size_t len = DARRAY_COUNT(array);
  size_t stride = DARRAY_STRIDE(array);
  if (index >= len) {
    fprintf(stderr, "%s, index outside the bounds of this array. length: %zu, index: %zu\n", __func__, len, stride);
    return array;
  }
  void* value_ptr = (void*)((char*)array + index * stride);
  if (dest) {
    memmove(dest, value_ptr, stride);
  }

  void* next_value_ptr = (void*)((char*)value_ptr + stride);
  if (index != len - 1) {
    memmove(value_ptr, next_value_ptr, stride * (len - index - 1));
  }

  size_t header_size = sizeof(struct dynamic_array_header);
  struct dynamic_array_header* header = (struct dynamic_array_header*)((char *)array - header_size);
  header->count--;
  return array;
}
