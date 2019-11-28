#ifndef _KD_TREE_H_
#define _KD_TREE_H_

#include "array.h"
#include "geom.h"

#define MAX_DIM 2

/**
 *  x_left < x <= x_right
 */
struct kd_node_t
{
    double x[MAX_DIM];
    struct kd_node_t *left, *right;
};

struct kd_node_t *make_kdtree(struct kd_node_t *t, int len, int i, int dim);

struct array kd_rangeQuery(struct kd_node_t *p, struct point query_p, double radius);
struct array kd_kNNQuery(struct kd_node_t *p, struct point query_p, int K);

#endif
