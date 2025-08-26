#ifndef _DARRAY_H_
#define _DARRAY_H_

#include <stddef.h>
  
struct dynamic_array_header {
  size_t capacity;
  size_t count;
  size_t stride;
  int factor;
};

#define DARRAY_TO_HEADER(DA) ((struct dynamic_array_header*)((char*)DA - sizeof(struct dynamic_array_header)))
#define HEADER_TO_DARRAY(H) ((char*)H + sizeof(dynamic_array_header))
#define DARRAY_COUNT(DA) (DARRAY_TO_HEADER(DA)->count)
#define DARRAY_CAP(DA) (DARRAY_TO_HEADER(DA)->capacity)
#define DARRAY_STRIDE(DA) (DARRAY_TO_HEADER(DA)->stride)

void darray_iterate(void* array, void (*func)(void* param));
void* _darray_create(size_t capacity, size_t stride, int factor);
void _darray_destroy(void** array);
void* _darray_push(void* array, const void* value_ptr);
void* _darray_insert_at(void* array, size_t index, const void* value_ptr);
void darray_pop(void* array, void* dest);
void* darray_pop_at(void* array, size_t index, void* dest);
int darray_is_invalid(void* array);

#define darray_create(type) \
  (type*)_darray_create(1, sizeof(type), 2)

#define darray_reserve(type, caps) \
  (type*)_darray_create(caps, sizeof(type), 2)

#define darray_push(array, value) \
  { \
    typeof(value) __tmp = value; \
    array = _darray_push(array, (const void*)&__tmp); \
  }

#define darray_insert_at(array, index, value) \
  { \
    typeof(value) __tmp = value; \
    array = _darray_insert_at(array, index, (const void*)&__tmp); \
  }

#define darray_destroy(parray) \
  { \
    _darray_destroy((void**)parray); \
  }

#define DEFINE_DARRAY(name, type) \
	type* name = darray_create(type);

#define DEFINE_DARRAY_RESERVE(name, type, cap) \
	type* name = darray_reserve(type, caps);

#endif
