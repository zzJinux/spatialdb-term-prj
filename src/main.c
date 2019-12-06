#include "array.h"
#include "geom.h"
#include "query_utils.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static struct array runQuery(FILE *listFile, FILE *paramsFile, char const *output_dir, struct array (*queryOp)(struct array, FILE *)) {
    struct array arr = read_points_comma(listFile);
    struct array timer_values = create_array(0, sizeof(double));

    int i = 0;
    while (!feof(paramsFile)) {
        clock_t start, end;

        start = clock();
        struct array out_points = queryOp(arr, paramsFile);
        end = clock();

        double delta = 1000.0*(end - start)/CLOCKS_PER_SEC;
        append_array(&timer_values, &delta);

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

    return timer_values;
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

    struct array (*queryOp)(struct array, FILE *);

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

    struct array timer_values = runQuery(listFile, paramsFile, queryOutDir, queryOp);

    for(int i=0; i<timer_values.len; ++i) {
        double *dd = (double *)timer_values.buf + i;
        printf("%lf\n", *dd);
    }

    fclose(listFile);
    fclose(paramsFile);

    return 0;
}
