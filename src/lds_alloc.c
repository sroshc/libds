#include "lds_alloc.h"

#include <stdlib.h>

struct lds_allocator GLOBAL_LDS_ALLOC = {
    .malloc = malloc, .realloc = realloc, .free = free};

struct lds_allocator get_lds_allocator(void *(*c_malloc)(size_t),
                                       void *(*c_realloc)(void *, size_t),
                                       void (*c_free)(void *)) {
  struct lds_allocator ret;
  ret.malloc = c_malloc;
  ret.realloc = c_realloc;
  ret.free = c_free;

  return ret;
}

inline void *lds_malloc(size_t size, struct lds_allocator *alloc_nullable) {
  return alloc_nullable == NULL ? GLOBAL_LDS_ALLOC.malloc(size)
                                : alloc_nullable.malloc(size);
}

inline void *lds_realloc(void *ptr, size_t size,
                         struct lds_allocator *alloc_nullable) {
  return alloc_nullable == NULL ? GLOBAL_LDS_ALLOC.realloc(ptr, size)
                                : alloc_nullable.realloc(ptr, size);
}
