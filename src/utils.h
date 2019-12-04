#ifndef _UTILS_H_
#define _UTILS_H_

#include "array.h"
#include "geom.h"
#include <stdio.h>

inline static struct array read_points(FILE *in) {
    struct array point_array = create_array(0, sizeof(struct point));
    while(!feof(in)) {
        struct point p;
        fscanf(in, "%lf %lf ", &p.x, &p.y);
        append_array(&point_array, &p);
    }

    return point_array;
}

inline static int sgn(double x) {
    return (.0 < x) - (x < .0);
}

#endif
