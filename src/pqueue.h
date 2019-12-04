#ifndef _PQUEUE_H_
#define _PQUEUE_H_

#include "array.h"

struct pqueue {
    struct array _arr;
    int (*_cmp)(void *, void const *, void const *);
    void *_aux;
};

struct pqueue create_pq(size_t, int (*)(void *, void const *, void const *), void *);
void destroy_pq(struct pqueue *pq);
size_t pq_size(struct pqueue *pq);
void pq_push(struct pqueue *pq, void *elem);
void pq_pop(struct pqueue *pq);
void *pq_top(struct pqueue *pq);

#endif
