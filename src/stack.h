#ifndef _STACK_H_
#define _STACK_H_

#include "array.h"

static void stack_push(struct array *arr, void *elem) {
    append_array(arr, elem);
}

static void stack_pop(struct array *arr) {
    --arr->len;
}

static void *stack_top(struct array *arr) {
    return (unsigned char *)arr->buf + arr->elem_size * (arr->len-1);
}

#endif
