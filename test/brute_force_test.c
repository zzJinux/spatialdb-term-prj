#include "brute_force.h"
#include "geom.h"
#include "io_utils.h"
#include <stdio.h>
#include <assert.h>

void testRangeQuery(FILE *listFile, FILE *rqAnsFile);
void testkNNQuery(FILE *listFile, FILE *kNNAnsFile);

int main(int argc, char *argv[]) {
    assert(argc == 4);
    FILE *listFile = fopen(argv[1], "r");
    assert(listFile != NULL);
    FILE *rqTestcase = fopen(argv[2], "r");
    assert(rqTestcase != NULL);
    FILE *kNNTestcase = fopen(argv[3], "r");
    assert(kNNTestcase != NULL);

    testRangeQuery(listFile, rqTestcase);
    testkNNQuery(listFile, kNNTestcase);

    fclose(listFile);
    fclose(kNNTestcase);
    fclose(rqTestcase);
    return 0;
}

void testRangeQuery(FILE *listFile, FILE *tcFile) {
    rewind(listFile);
    rewind(tcFile);

    struct array arr = read_points(listFile);

    struct point query_p;
    double radi;
    fscanf(tcFile, "%lf %lf %lf ", &query_p.x, &query_p.y, &radi);
    struct array out_points = bf_rangeQuery(arr, query_p, radi);
    sort_array(&out_points, comparePoints);

    struct array ans_points = create_array(0, sizeof(struct point));
    while(!feof(tcFile)) {
        struct point p;
        fscanf(tcFile, "%lf %lf ", &p.x, &p.y);
        append_array(&ans_points, &p);
    }
    sort_array(&ans_points, comparePoints);

    assert(out_points.len == ans_points.len);
    struct point *_out_p = out_points.buf;
    struct point *_ans_p = ans_points.buf;
    for(int i=0; i<out_points.len; ++i) {
        assert(_out_p[i].x == _ans_p[i].x && _out_p[i].y == _ans_p[i].y);
    }

    destroy_array(&arr);
    destroy_array(&out_points);
    destroy_array(&ans_points);
}

void testkNNQuery(FILE *listFile, FILE *tcFile) {
    rewind(listFile);
    rewind(tcFile);

    struct array arr = read_points(listFile);

    struct point query_p;
    int k;
    fscanf(tcFile, "%lf %lf %d ", &query_p.x, &query_p.y, &k);
    struct array out_points = bf_kNNQuery(arr, query_p, k);
    sort_array(&out_points, comparePoints);

    struct array ans_points = create_array(0, sizeof(struct point));
    while(!feof(tcFile)) {
        struct point p;
        fscanf(tcFile, "%lf %lf ", &p.x, &p.y);
        append_array(&ans_points, &p);
    }
    sort_array(&ans_points, comparePoints);

    assert(out_points.len == ans_points.len);
    struct point *_out_p = out_points.buf;
    struct point *_ans_p = ans_points.buf;
    for(int i=0; i<out_points.len; ++i) {
        assert(_out_p[i].x == _ans_p[i].x && _out_p[i].y == _ans_p[i].y);
    }

    destroy_array(&arr);
    destroy_array(&out_points);
    destroy_array(&ans_points);
}
