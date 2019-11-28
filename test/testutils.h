#ifndef _TESTUTILS_H_
#define _TESTUTILS_H_

#include "geom.h"
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

inline static void assertPointEquals(struct point const *lhs, struct point const *rhs) {
    assert(comparePoints(lhs, rhs) == 0);
}

inline static void assertPointArrayEquals(struct array const *lhs, struct array const *rhs) {
    size_t n1 = lhs->len, n2 = rhs->len;
    struct point *arr1 = (struct point *)lhs->buf, *arr2 = (struct point *)rhs->buf;

    assert(n1 == n2);
    for(int i=0; i<n1; ++i) {
        assertPointEquals(&arr1[i], &arr2[i]);
    }
}

inline static void testQuery(
    FILE *listFile, FILE *tcFile,
    struct array (*queryOp)(struct array, FILE *)
) {
    rewind(listFile);
    rewind(tcFile);

    struct array arr = read_points(listFile);

    /* tcFile */
    /* First is the query parameter */
    struct array out_points = queryOp(arr, tcFile);
    sort_array(&out_points, comparePoints);

    /* tcFile */
    /* Next is the answer */
    struct array ans_points = read_points(tcFile);
    sort_array(&ans_points, comparePoints);

    assertPointArrayEquals(&out_points, &ans_points);

    destroy_array(&arr);
    destroy_array(&out_points);
    destroy_array(&ans_points);
}

#endif
