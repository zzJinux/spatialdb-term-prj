#include "array.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUF_WND (1024)

struct array create_array(size_t len, size_t sz) {
    struct array arr;
    size_t _caplen = ((len + BUF_WND-1)/BUF_WND)*BUF_WND;
    if(_caplen == 0) _caplen = BUF_WND;
    arr.len = len;
    arr.elem_size = sz;
    arr._caplen = _caplen;
    arr.buf = malloc(_caplen * sz);
    return arr;
}

void resize_array(struct array *arr, size_t len) {
    if(arr->_caplen < len) {
        size_t _caplen = ((len + BUF_WND-1)/BUF_WND)*BUF_WND;
        arr->buf = realloc(arr->buf, _caplen * arr->elem_size);
        arr->_caplen = _caplen;
    }
    arr->len = len;
}

void append_array(struct array *arr, void *elem) {
    size_t new_len = arr->len + 1;
    resize_array(arr, new_len);
    unsigned char *dest = (unsigned char *)arr->buf + (new_len-1) * arr->elem_size;
    memcpy(dest, elem, arr->elem_size);
}

void sort_array(struct array *arr, int (*cmp)(void const *, void const *)) {
    qsort(arr->buf, arr->len, arr->elem_size, cmp);
}

void destroy_array(struct array *arr) {
    arr->len = 0;
    free(arr->buf);
    arr->buf = NULL;
    arr->_caplen = 0;
}
