#ifndef LDS_ALLOC_
#define LDS_ALLOC_

#include "lds_defs.h"

#include <stdlib.h>

struct lds_allocator{
    void* (*malloc)(size_t);
    void* (*realloc)(void*, size_t);
    void (*free)(void*);
};

struct lds_allocator get_lds_allocator(void* (*c_malloc)(size_t), void* (*c_realloc)(void*, size_t), void (*c_free)(void*));

inline void* lds_malloc(struct lds_allocator* alloc_nullable);

inline void* lds_realloc(void* ptr, size_t size, struct lds_allocator* alloc_nullable);

inline void* lds_free(void* ptr, struct lds_allocator* alloc_nullable);

extern struct lds_allocator GLOBAL_LDS_ALLOC;

#endif