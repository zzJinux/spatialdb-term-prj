#include "brute_force.h"

struct array bf_rangeQuery(
    struct array point_list,
    struct point query_p, double radi
) {
    struct array result = create_array(0, sizeof(struct point));
    int i;
    int N = point_list.len;
    struct point *points = point_list.buf;
    for(i=0; i<N; ++i) {
        if(dist(query_p, points[i]) < radi) {
            append_array(&result, &points[i]);
        }
    }

    return result;
}

struct array bf_kNNQuery(
    struct array point_list,
    struct point query_p, int k
) {
    int i, j;

    struct array result = create_array(k, sizeof(struct point));
    struct array d_arr = create_array(k, sizeof(double));

    struct point *kNN_points = result.buf;
    double *distances = d_arr.buf;
    for(i=0; i<k; ++i) {
        kNN_points[i].x = INFINITY;
        kNN_points[i].y = INFINITY;
        distances[i] = INFINITY;
    }

    int N = point_list.len;
    struct point *points = point_list.buf;

    for(i=0; i<N; ++i) {
        double d = dist(query_p, points[i]);
        for(j=0; j<k; ++j) {
            if(distances[j] > d) break;
        }
        if(j == k) continue;
        int jj;
        for(jj = k-1; jj > j; --jj) {
            if(distances[jj-1] == INFINITY) continue;
            kNN_points[jj] = kNN_points[jj-1];
            distances[jj] = distances[jj-1];
        }
        kNN_points[j] = points[i];
        distances[j] = d;
    }

    destroy_array(&d_arr);
    return result;
}
