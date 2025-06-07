#ifndef LDS_ARRAY_
#define LDS_ARRAY_

#include "lds_defs.h"

#include <stddef.h>

struct lds_array{
    void* arr;
    size_t elem_size;
    size_t curr_size;
    size_t max_size;
};

LDS_RET new_lds_array(lds_array* ret, size_t init_size, size_t elem_size);

static LDS_RET resize_lds_array(lds_array* lds_arr, size_t size);

LDS_RET add_lds_array(lds_array* lds_arr, const void* item);