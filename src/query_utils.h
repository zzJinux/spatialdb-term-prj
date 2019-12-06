#ifndef _QUERY_UTILS_H_
#define _QUERY_UTILS_H_

#include "brute_force.h"
#include "KDTree.h"
#include "RTree.h"
#include <stdio.h>
#include <time.h>

static struct array run_bf_rangeQuery(struct array arr, FILE *tcFile, double *delta) {
    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);

    clock_t start, end;
    start = clock();
    struct array results = bf_rangeQuery(arr, query_p, radi);
    end = clock();

    *delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
    return results;
}

static struct array run_bf_kNNQuery(struct array arr, FILE *tcFile, double *delta) {
    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);

    clock_t start, end;
    start = clock();
    struct array results = bf_kNNQuery(arr, query_p, k);
    end = clock();

    *delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
    return results;
}

static struct array kdnodesFromPoints(struct array arr) {
    size_t N = arr.len;
    struct array kdnodes_array = create_array(N, sizeof(struct kd_node_t));
    for(int i=0; i<N; ++i) {
        struct kd_node_t *kd_node = (struct kd_node_t *)kdnodes_array.buf + i;
        struct point *point = (struct point *)arr.buf + i;

        kd_node->x[0] = point->x;
        kd_node->x[1] = point->y;
    }

    return kdnodes_array;
}

static struct array run_kd_rangeQuery(struct array arr, FILE *tcFile, double *delta) {
    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);

    struct array kdnodes_array = kdnodesFromPoints(arr);
    struct kd_node_t *kd_root = make_kdtree((struct kd_node_t *)kdnodes_array.buf, arr.len, 0, 2);
    clock_t start, end;
    start = clock();
    struct array queryResult = kd_rangeQuery(kd_root, query_p, radi);
    end = clock();

    *delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
    destroy_array(&kdnodes_array);
    return queryResult;
}

static struct array run_kd_kNNQuery(struct array arr, FILE *tcFile, double *delta) {
    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);

    struct array kdnodes_array = kdnodesFromPoints(arr);
    struct kd_node_t *kd_root = make_kdtree((struct kd_node_t *)kdnodes_array.buf, arr.len, 0, 2);
    clock_t start, end;
    start = clock();
    struct array queryResult = kd_kNNQuery(kd_root, query_p, k);
    end = clock();

    *delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
    destroy_array(&kdnodes_array);
    return queryResult;
}

static RTREENODE *RTNodeFromPoints(struct array arr) {
    RTREENODE *root = RTreeCreate();
    for(int i=0; i<arr.len; ++i) {
        struct point *p = (struct point *)arr.buf + i;
        RTREEMBR mbr;
        mbr.bound[0] = mbr.bound[2] = p->x;
        mbr.bound[1] = mbr.bound[3] = p->y;
        RTreeInsertRect(&mbr, i+10, &root, 0);
    }
    return root;
}

static struct array run_rt_rangeQuery(struct array arr, FILE *tcFile, double *delta) {
    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);

    RTREENODE *root = RTNodeFromPoints(arr);
    clock_t start, end;
    start = clock();
    struct array results = RT_rangeQuery(root, query_p, radi);
    end = clock();

    *delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
    RTreeDestroy(root);
    return results;
}

static struct array run_rt_kNNQuery(struct array arr, FILE *tcFile, double *delta) {
    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);

    RTREENODE *root = RTNodeFromPoints(arr);
    clock_t start, end;
    start = clock();
    struct array results = RT_kNNQuery(root, query_p, k);
    end = clock();

    *delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
    RTreeDestroy(root);
    return results;
}



#endif
