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

inline static double dist(struct point const A, struct point const B) {
    double xd = B.x - A.x;
    double yd = B.y - A.y;
    return sqrt(xd*xd + yd*yd);
}

inline static double dist_p(struct point const *A, struct point const *B) {
    double xd = B->x - A->x;
    double yd = B->y - A->y;
    return sqrt(xd*xd + yd*yd);
}

inline static int comparePoints(void const *lhs, void const *rhs) {
    struct point lp = *(struct point const *)lhs;
    struct point rp = *(struct point const *)rhs;

    if(fabs(lp.x - rp.x) > 1e-6) return lp.x < rp.x ? -1 : 1;
    else if(fabs(lp.y - rp.y) > 1e-6) return lp.y < rp.y ? -1 : 1;
    else return 0;
}

inline static double MINDIST(struct point p, struct rect r) {
    double dx = .0;
    double dy = .0;

    if(p.x < r.min_x) {
        dx = r.min_x - p.x;
    }
    else if(p.x > r.max_x) {
        dx = p.x - r.max_x;
    }

    if(p.y < r.min_y) {
        dy = r.min_y - p.y;
    }
    else if(p.y > r.max_y) {
        dy = p.y - r.max_y;
    }

    return sqrt(dx*dx + dy*dy);
}

#endif
