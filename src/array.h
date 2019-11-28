#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "stddef.h"

struct array {
    size_t len, elem_size;
    size_t _caplen;
    void *buf;
};

struct array create_array(size_t len, size_t sz);
void resize_array(struct array *, size_t len);
void append_array(struct array *, void *elem);
void sort_array(struct array *, int (*cmp)(void const *, void const *));
void destroy_array(struct array *);

#endif
