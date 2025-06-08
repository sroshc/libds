#ifndef LDS_ARRAY_
#define LDS_ARRAY_

#include "lds_defs.h"

#include <stddef.h>

#define LDS_ARRAY_DEFAULT_SIZE 4

struct lds_array{
    void* arr;
    size_t elem_size;
    size_t curr_size;
    size_t max_size;
};

LDS_RET new_lds_array(lds_array* ret, size_t init_size, size_t elem_size);

LDS_RET add_lds_array(lds_array* lds_arr, const void* item);

LDS_RET set_lds_array(lds_array* lds_arr, size_t pos, void* element);

void *get_lds_array_ptr(struct lds_array *lds_arr, size_t pos);

LDS_RET get_lds_array(struct lds_array *lds_arr, size_t pos, void *dest);

LDS_RET remove_lds_array(struct lds_array *lds_arr, size_t pos,
                         void *element_nullable);

LDS_RET clear_lds_array(struct lds_array *lds_arr, size_t size,
                        struct lds_allocator *alloc_nullable);

void destroy_lds_array(struct lds_array *lds_arr,
                          struct lds_allocator *alloc_nullable);