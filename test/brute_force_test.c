#include "brute_force.h"
#include "geom.h"
#include "io_utils.h"
#include "testutils.h"
#include <stdio.h>
#include <assert.h>

static struct array rangeQueryFn(struct array arr, FILE *tcFile) {
    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);
    return bf_rangeQuery(arr, query_p, radi);
}

static struct array kNNQueryFn(struct array arr, FILE *tcFile) {
    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);
    return bf_kNNQuery(arr, query_p, k);
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
