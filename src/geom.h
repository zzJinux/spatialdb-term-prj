#ifndef _TYPES_H_
#define _TYPES_H_

#include "array.h"
#include <stdbool.h>
#include <math.h>

struct point {
    double x, y;
};

struct rect {
    double min_x, min_y;
    double max_x, max_y;
};

inline static double dist(struct point A, struct point B) {
    double xd = B.x - A.x;
    double yd = B.y - A.y;
    return sqrt(xd*xd + yd*yd);
}

inline static int comparePoints(void const *lhs, void const *rhs) {
    struct point lp = *(struct point const *)lhs;
    struct point rp = *(struct point const *)rhs;

    if(fabs(lp.x - rp.x) > 1e-6) return lp.x < rp.x ? -1 : 1;
    else if(fabs(lp.y - rp.y) > 1e-6) return lp.y < rp.y ? -1 : 1;
    else return 0;
}

#endif
