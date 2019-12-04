#include "pqueue.h"
#include <assert.h>
#include <stdio.h>

static int compare_ints(void const *a, void const *b) {
    return *(int const *)a - *(int const *)b;
}

static void printInts(struct pqueue pq) {
    int n = pq_size(&pq);
    putchar('>');
    for(int i=1; i<=n; ++i) {
        int *arr = pq._arr.buf;
        printf(" %d", arr[i]);
    }
    putchar('\n');

}

int main(void) {
    int int_values[] = { 5, 3, 1, 7, 4, 9 };
    size_t n = sizeof(int_values)/sizeof(int);

    struct pqueue pq = create_pq(sizeof(int), compare_ints);
    int inter_results[] = { 5, 5, 5, 7, 7, 9 };
    for(int i=0; i<n; ++i) {
        pq_push(&pq, &int_values[i]);
        int t = *(int const *)pq_top(&pq);
        int sz = pq_size(&pq);
        assert(t == inter_results[i]);
        assert(sz == i+1);
    }

    int before_pop_results[] = { 9, 7, 5, 4, 3, 1 };
    for(int i=0; i<n; ++i) {
        printInts(pq);
        int t = *(int const *)pq_top(&pq);
        assert(t == before_pop_results[i]);
        pq_pop(&pq);
        int sz = pq_size(&pq);
        assert(sz == n-i-1);
    }
    return 0;
}
