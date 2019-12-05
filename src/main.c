#include "array.h"
#include "geom.h"
#include "utils.h"
#include <stdio.h>

static void runQuery(FILE *listFile, struct array (*queryOp)(struct array, FILE *)) {
    struct array arr = read_points(listFile);

    struct array out_points = queryOp(arr, stdout);
    sort_array(&out_points, comparePoints);

    for(int i=0; i<out_points.len; ++i) {
        struct point *p = (struct point *)out_points.buf + i;
        printf("%.6lf %.6lf\n", p->x, p->y);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("usage:\n\t<executable> <type> <operation> <path to list file>\n");
        return -1;
    }

    char const *type = argv[1];
    char const *operation = argv[2];
    char const *listPath = argv[3];

    return 0;
}
