#include "lds_array.h"

#include "lds_alloc.h"
#include "lds_defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


LDS_RET new_lds_array(struct lds_array* a_ret, size_t init_size, size_t elem_size, struct lds_allocator* alloc_nullable){
    struct lds_array ret;

    ret.curr_size = 0;
    ret.max_size = init_size == 0? 4: init_size;
    ret.elem_size = elem_size;
    
    ret.arr = lds_malloc(elem_size * init_size, alloc_nullable);
    if(ret.arr == NULL){
        fprintf(stderr, "lds_array initialization malloc failure");
        return LDS_ALLOC_ERROR;
    }
    
    *a_ret = ret;
    return LDS_SUCCESS;    
}

static LDS_RET resize_lds_array(lds_array* lds_arr, size_t size, struct lds_allocator* alloc_nullable){
    void* resized_buf = lds_realloc(lds_arr->lds_arr.arr, size, alloc_nullable);

    if(resized_buf == NULL){
        fprintf(stderr, "lds_array resize realloc failure");
        return LDS_ALLOC_ERROR;
    }

    lds_arr->arr = resized_buf;
    return LDS_SUCCESS;
}


LDS_RET add_lds_array(lds_array* lds_arr, void* element, struct lds_allocator* alloc_nullable){
    if(element == NULL){
        fprintf(stderr, "add lds_array failed, null element");
        return LDS_ERROR;
    }

    if(lds_arr->curr_size + 1 > lds_arr->max_size){ // Array exceeds maximum size.
        int res = resize_lds_array(lds_arr, lds_arr->max_size * 2, alloc_nullable);
        if(LDS_SUCCESS != res){
            return res;
        }
        
        lds_arr->max_size *= 2;

    }

    memcpy((char*) lds_arr->arr + lds_arr->elem_size * lds_arr->curr_size, element, lds_array->elem_size);

    lds_arr->curr_size++;

    return LDS_SUCCESS;
}
