#include "RTree.h"
#include "geom.h"
#include "utils.h"
#include "testutils.h"
#include <stdio.h>
#include <assert.h>

static RTREENODE *buildFromPoints(struct array arr) {
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

static struct array rangeQueryFn(struct array arr, FILE *tcFile) {
    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);

    RTREENODE *root = buildFromPoints(arr);
    struct array results = RT_rangeQuery(root, query_p, radi);

    RTreeDestroy(root);
    return results;
}

static struct array kNNQueryFn(struct array arr, FILE *tcFile) {
    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);

    RTREENODE *root = buildFromPoints(arr);
    struct array results = RT_kNNQuery(root, query_p, k);

    RTreeDestroy(root);
    return results;
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
