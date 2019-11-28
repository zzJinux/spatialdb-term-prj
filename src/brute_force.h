#ifndef _BRUTE_FORCE_H_
#define _BRUTE_FORCE_H_

#include "geom.h"
#include "array.h"

struct array bf_rangeQuery(
    struct array point_list,
    struct point query_p, double radi
);

struct array bf_kNNQuery(
    struct array point_list,
    struct point query_p, int k
);

#endif
