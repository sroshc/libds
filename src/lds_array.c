#include "lds_array.h"

#include "lds_alloc.h"
#include "lds_defs.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LDS_RET new_lds_array(struct lds_array *o_ret, size_t init_size,
                      size_t elem_size,
                      const struct lds_allocator *alloc_nullable) {
  struct lds_array ret;

  ret.curr_size = 0;
  ret.max_size = init_size == 0 ? LDS_ARRAY_DEFAULT_SIZE : init_size;
  ret.elem_size = elem_size;

  ret.arr = lds_malloc(elem_size * ret.max_size, alloc_nullable);
  if (ret.arr == NULL) {
    fprintf(stderr, "lds_array initialization malloc failure\n");
    return LDS_ALLOC_ERROR;
  }

  *o_ret = ret;
  return LDS_SUCCESS;
}

static LDS_RET resize_lds_array(struct lds_array *lds_arr, size_t size,
                                const struct lds_allocator *alloc_nullable) {
  void *resized_buf = lds_realloc(lds_arr->arr, lds_arr->elem_size * size, alloc_nullable);

  if (resized_buf == NULL) {
    fprintf(stderr, "lds_array resize realloc failure\n");
    return LDS_ALLOC_ERROR;
  }

  lds_arr->arr = resized_buf;
  lds_arr->max_size = size;
  return LDS_SUCCESS;
}

static inline void write_lds_array(struct lds_array *lds_arr, size_t pos,
                                   const void *element) {
  memcpy((char *)lds_arr->arr + lds_arr->elem_size * pos, element,
         lds_arr->elem_size);
}

static inline void read_lds_array(struct lds_array *lds_arr, size_t pos,
                                  void *dest) {
  memcpy(dest, (char *)lds_arr->arr + lds_arr->elem_size * pos,
         lds_arr->elem_size);
}

LDS_RET add_lds_array(struct lds_array *lds_arr, const void *element,
                      const struct lds_allocator *alloc_nullable) {
  if (element == NULL) {
    fprintf(stderr, "add lds_array failed, null element\n");
    return LDS_ERROR;
  }

  if (lds_arr->curr_size + 1 >
      lds_arr->max_size) { // If array exceeds maximum size, double the array's
                           // size.
    LDS_RET res =
        resize_lds_array(lds_arr, lds_arr->max_size * 2, alloc_nullable);
    if (LDS_SUCCESS != res) {
      return res;
    }

  }

  write_lds_array(lds_arr, lds_arr->curr_size, element);
  lds_arr->curr_size++;

  return LDS_SUCCESS;
}

LDS_RET set_lds_array(struct lds_array *lds_arr, size_t pos,
                      const void *element) {
  if (lds_arr->curr_size <= pos) {
    return LDS_OUT_OF_BOUNDS_ERROR;
  }

  write_lds_array(lds_arr, pos, element);

  return LDS_SUCCESS;
}

void *get_lds_array_ptr(struct lds_array *lds_arr, size_t pos) {
  if (lds_arr->curr_size <= pos) {
    return NULL;
  }

  return (void *)((char *)lds_arr->arr + lds_arr->elem_size * pos);
}

LDS_RET get_lds_array(struct lds_array *lds_arr, size_t pos, void *dest) {
  if (lds_arr->curr_size <= pos) {
    return LDS_OUT_OF_BOUNDS_ERROR;
  }

  read_lds_array(lds_arr, pos, dest);

  return LDS_SUCCESS;
}

LDS_RET remove_lds_array(struct lds_array *lds_arr, size_t pos,
                         void *element_nullable) {
  if (lds_arr->curr_size <= pos) {
    return LDS_OUT_OF_BOUNDS_ERROR;
  }

  if (element_nullable != NULL) {
    read_lds_array(lds_arr, pos, element_nullable);
  }

  if (lds_arr->curr_size - 1 >
      pos) { // Shift elements down one if not removing the last element.
    void *dest = (char *)lds_arr->arr + lds_arr->elem_size * pos;
    void *src = (char *)dest + lds_arr->elem_size;
    size_t shifted_bytes = lds_arr->elem_size * (lds_arr->curr_size - pos - 1);
    memmove(dest, src, shifted_bytes);
  }

  lds_arr->curr_size--;
  return LDS_SUCCESS;
}

LDS_RET clear_lds_array(struct lds_array *lds_arr, size_t size,
                        struct lds_allocator *alloc_nullable) {
  lds_free(lds_arr->arr, alloc_nullable);

  lds_arr->curr_size = 0;
  lds_arr->max_size = size == 0 ? LDS_ARRAY_DEFAULT_SIZE : size;

  lds_arr->arr = lds_malloc(lds_arr->elem_size * lds_arr->max_size, alloc_nullable);

  if (lds_arr->arr == NULL) {
    fprintf(stderr, "lds_array clear malloc failure\n");
    return LDS_ALLOC_ERROR;
  }

  return LDS_SUCCESS;
}

void destroy_lds_array(struct lds_array *lds_arr,
                          struct lds_allocator *alloc_nullable) {
  lds_free(lds_arr->arr, alloc_nullable);

  lds_arr->arr = NULL;
  lds_arr->max_size = 0;
  lds_arr->curr_size = 0;
  lds_arr->elem_size = 0;

}