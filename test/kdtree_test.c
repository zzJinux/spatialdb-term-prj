#include "KDTree.h"
#include "geom.h"
#include "utils.h"
#include "testutils.h"
#include <stdio.h>
#include <assert.h>

static struct array fromPointArray(struct array arr) {
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

static struct array rangeQueryFn(struct array arr, FILE *tcFile) {
    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);

    struct array kdnodes_array = fromPointArray(arr);
    struct kd_node_t *kd_root = make_kdtree(kdnodes_array.buf, arr.len, 0, 2);

    struct array queryResult = kd_rangeQuery(kd_root, query_p, radi);
    destroy_array(&kdnodes_array);

    return queryResult;
}

static struct array kNNQueryFn(struct array arr, FILE *tcFile) {
    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);

    struct array kdnodes_array = fromPointArray(arr);
    struct kd_node_t *kd_root = make_kdtree(kdnodes_array.buf, arr.len, 0, 2);

    struct array queryResult = kd_kNNQuery(kd_root, query_p, k);
    destroy_array(&kdnodes_array);

    return queryResult;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    FILE *listFile = fopen(argv[1], "r");
    assert(listFile != NULL);
    FILE *rqTestcase = fopen(argv[2], "r");
    assert(rqTestcase != NULL);
    FILE *kNNTestcase = fopen(argv[3], "r");
    assert(kNNTestcase != NULL);


    testQuery(listFile, rqTestcase, rangeQueryFn);
    testQuery(listFile, kNNTestcase, kNNQueryFn);

    fclose(listFile);
    fclose(kNNTestcase);
    fclose(rqTestcase);
    return 0;
}
