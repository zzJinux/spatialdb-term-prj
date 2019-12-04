#include "pqueue.h"
#include <string.h>

struct pqueue create_pq(size_t elem_sz, int (*cmp)(void *, void const *, void const *), void *aux) {
    struct pqueue pq;
    pq._arr = create_array(1, elem_sz);
    pq._cmp = cmp;
    pq._aux = aux;
    return pq;
}

void destroy_pq(struct pqueue *pq) {
    destroy_array(&pq->_arr);
}

size_t pq_size(struct pqueue *pq) {
    return pq->_arr.len - 1;
}

void pq_push(struct pqueue *pq, void *elem) {
    resize_array(&pq->_arr, pq->_arr.len + 1);
    size_t elem_size = pq->_arr.elem_size;
    int i = pq->_arr.len - 1;
    while(i > 1 && pq->_cmp(pq->_aux, elem, GET(pq->_arr, i/2)) > 0) {
        memcpy(GET(pq->_arr, i), GET(pq->_arr, i/2), elem_size);
        i/=2;
    }
    memcpy(GET(pq->_arr, i), elem, elem_size);
}

void pq_pop(struct pqueue *pq) {
    // ptr to last element
    void *p = GET(pq->_arr, pq->_arr.len - 1);
    size_t post_arr_sz = pq->_arr.len - 1;
    size_t elem_size = pq->_arr.elem_size;

    int i = 2;
    while(i < post_arr_sz) {
        if(i+1 < post_arr_sz && pq->_cmp(pq->_aux, GET(pq->_arr, i+1), GET(pq->_arr, i)) > 0) {
            ++i;
        }

        if(pq->_cmp(pq->_aux, p, GET(pq->_arr, i)) >= 0) break;

        memcpy(GET(pq->_arr, i/2), GET(pq->_arr, i), elem_size);
        i *= 2;
    }

    memcpy(GET(pq->_arr, i/2), p, elem_size);
    resize_array(&pq->_arr, post_arr_sz);
}

void *pq_top(struct pqueue *pq) {
    return GET(pq->_arr, 1);
}
