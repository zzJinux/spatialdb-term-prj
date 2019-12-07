#include "array.h"
#include "geom.h"
#include "query_utils.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

struct metric {
    struct array timer_values;
    struct array hit_values;
};

static struct metric runQuery(FILE *listFile, FILE *paramsFile, char const *output_dir, struct array (*queryOp)(struct array, FILE *, double *, int *)) {
    struct metric metric;

    struct array arr = read_points_comma(listFile);
    metric.timer_values = create_array(0, sizeof(double));
    metric.hit_values = create_array(0, sizeof(int));

    int i = 0;
    while (!feof(paramsFile)) {
        clock_t start, end;

        double delta;
        int nVisits;
        struct array out_points = queryOp(arr, paramsFile, &delta, &nVisits);

        append_array(&metric.timer_values, &delta);
        append_array(&metric.hit_values, &nVisits);

        sort_array(&out_points, comparePoints);

        char filename[1024];
        sprintf(filename, "%s/output_%03d.txt", output_dir, i);
        FILE *oFile = fopen(filename, "w");
        for (int i = 0; i < out_points.len; ++i) {
            struct point *p = (struct point *)out_points.buf + i;
            fprintf(oFile, "%.6lf %.6lf\n", p->x, p->y);
        }

        fclose(oFile);
        destroy_array(&out_points);

        ++i;
    }

    destroy_array(&arr);

    return metric;
}

int main(int argc, char *argv[]) {
    if(argc != 6) {
        printf("usage:\n\t<executable> <type> <operation> <list file> <query params file> <query output dir>\n");
        return -1;
    }

    char const *type = argv[1];
    char const *operation = argv[2];
    char const *listFilePath = argv[3];
    char const *paramsFilePath = argv[4];
    char const *queryOutDir = argv[5];

    struct array (*queryOp)(struct array, FILE *, double *, int *);

    if(strcmp(type, "bruteforce") == 0) {
        if(strcmp(operation, "range") == 0) {
            queryOp = run_bf_rangeQuery;
        }
        else if(strcmp(operation, "kNN") == 0) {
            queryOp = run_bf_kNNQuery;
        }
        else {
            printf("unsupported operation: '%s'\n", operation);
            return -1;
        }
    }
    else if(strcmp(type, "kdtree") == 0) {
        if(strcmp(operation, "range") == 0) {
            queryOp = run_kd_rangeQuery;
        }
        else if(strcmp(operation, "kNN") == 0) {
            queryOp = run_kd_kNNQuery;
        }
        else {
            printf("unsupported operation: '%s'\n", operation);
            return -1;
        }
    }
    else if(strcmp(type, "rtree") == 0) {
        if(strcmp(operation, "range") == 0) {
            queryOp = run_rt_rangeQuery;
        }
        else if(strcmp(operation, "kNN") == 0) {
            queryOp = run_rt_kNNQuery;
        }
        else {
            printf("unsupported operation: '%s'\n", operation);
            return -1;
        }
    }
    else {
        printf("unsupported type: '%s'\n", type);
        return -1;
    }

    FILE *listFile = fopen(listFilePath, "r");
    FILE *paramsFile = fopen(paramsFilePath, "r");

    struct metric metric = runQuery(listFile, paramsFile, queryOutDir, queryOp);

    for(int i=0; i<metric.timer_values.len; ++i) {
        double *dd = (double *)metric.timer_values.buf + i;
        int *hh = (int *)metric.hit_values.buf + i;
        printf("%lf %d\n", *dd, *hh);
    }

    fclose(listFile);
    fclose(paramsFile);

    return 0;
}
