//
//  kdtree.c
//  
//
//  Created by Bigdata LAB on 2019/11/05.
//

#include "KDTree.h"
#include "geom.h"
#include "stack.h"
#include "array.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define COMPARE(a, b) ((a > b) ? a : b)

typedef void (*NoArgMethod)(void *);
typedef void (*OneArgMethod)(void *, void *);
typedef void *(*NoArgMethodRPtr)(void *);
typedef int (*NoArgMethodRInt)(void *);

struct candidate_node {
    struct kd_node_t *current_node;
    struct rect rect;
    int dim;
};

// 거리함수 정의.
inline static double kd_dist(struct kd_node_t *a, struct kd_node_t *b, int dim)
{
    double t, d = 0;
    while (dim--)
    {
        t = a->x[dim] - b->x[dim];
        d = d + t * t;
    }
    
    return sqrt(d);
}

// swap 함수 정의.
inline static void kd_swap(struct kd_node_t *x, struct kd_node_t *y)
{
    double tmp[MAX_DIM];
    memcpy(tmp, x->x, sizeof(tmp));
    memcpy(x->x, y->x, sizeof(tmp));
    memcpy(y->x, tmp, sizeof(tmp));
}

// 중앙값 찾는 함수 정의. 중앙값은 kdtree의 node split point를 찾을 때 사용된다.
inline static struct kd_node_t* find_median(struct kd_node_t *start, struct kd_node_t *end, int idx)
{
    if (end <= start) return NULL;
    if (end == start + 1)
        return start;
    
    struct kd_node_t *p, *store, *med;
    med = start + (end - start) / 2;
    double pivot;
    
    while (1)
    {
        pivot = med->x[idx];
        
        kd_swap(med, end-1);
        for(store = p = start; p < end; p++)
            if(p->x[idx] < pivot)
            {
                if(p != store)
                    kd_swap(p, store);
                store++;
            }
        kd_swap(store, end - 1);
        
        if(store->x[idx] == med->x[idx])
            return med;
        if (store > med) end = store;
        else start = store;
    }
}

// recursion으로 kdtree를 build하는 함수.
struct kd_node_t *make_kdtree(struct kd_node_t *t, int len, int i, int dim)
{
    struct kd_node_t *n;
    
    if(!len) return NULL;

    if((n = find_median(t, t+len, i)))
    {
        i = (i + 1) % dim;
        n->left = make_kdtree(t, n-t, i, dim);
        n->right = make_kdtree(n+1, t+len-(n+1), i, dim);
    }
    
    return n;
}

struct traverse_context {
    double radius;
    void *results;
    OneArgMethod query_hit;
    void *nodes;
    OneArgMethod nodes_push;
    NoArgMethod nodes_pop;
    NoArgMethodRInt nodes_size;
    NoArgMethodRPtr nodes_top;
};

static void traverse(struct kd_node_t *p, struct point query_p, struct traverse_context *ctx) {
    int const DIM = 2;

    struct candidate_node cand;
    cand.current_node = p;
    cand.rect.min_x = -INFINITY;
    cand.rect.min_y = -INFINITY;
    cand.rect.max_x = INFINITY;
    cand.rect.max_y = INFINITY;
    cand.dim = 0;
    ctx->nodes_push(ctx->nodes, &cand);

    struct kd_node_t query_kd;
    query_kd.x[0] = query_p.x;
    query_kd.x[1] = query_p.y;

    while(ctx->nodes_size(ctx->nodes)) {
        struct candidate_node *cand = (struct candidate_node *)ctx->nodes_top(ctx->nodes);
        struct kd_node_t *node = cand->current_node;
        struct rect rect = cand->rect;
        int dim_idx = cand->dim;
        ctx->nodes_pop(ctx->nodes);
        double d = kd_dist(node, &query_kd, 2);

        if(d < ctx->radius) {
            struct point hitp;
            hitp.x = node->x[0];
            hitp.y = node->x[1];
            ctx->query_hit(ctx->results, &hitp);
        }

        double min_x = rect.min_x, max_x = rect.max_x;
        double x = node->x[dim_idx];
        if(dim_idx == 1) {
            min_x = rect.min_y;
            max_x = rect.max_y;
        }

        if(node->left) {
            struct candidate_node new_cand;
            new_cand.current_node = node->left;
            new_cand.rect = rect;
            new_cand.dim = (dim_idx + 1) % DIM;
            struct rect *crect = &new_cand.rect;

            if(dim_idx == 0) {
                crect->min_x = min_x;
                crect->max_x = x;
            }
            else {
                crect->min_y = min_x;
                crect->max_y = x;
            }

            if(MINDIST(query_p, *crect) < ctx->radius) {
                ctx->nodes_push(ctx->nodes, &new_cand);
            }
        }

        if(node->right) {
            struct candidate_node new_cand;
            new_cand.current_node = node->right;
            new_cand.rect = rect;
            new_cand.dim = (dim_idx + 1) % DIM;
            struct rect *crect = &new_cand.rect;
            if(dim_idx == 0) {
                crect->min_x = x;
                crect->max_x = max_x;
            }
            else {
                crect->min_y = x;
                crect->max_y = max_x;
            }

            if(MINDIST(query_p, *crect) < ctx->radius) {
                ctx->nodes_push(ctx->nodes, &new_cand);
            }
        }
    }

    return;
}

/* --------------- METHODS FOR RANGE QUERY --------------- */

static void _rq_push(void *st, void *elem) {
    stack_push(st, elem);
}

static void _rq_pop(void *st) {
    stack_pop(st);
}

static int _rq_size(void *st) {
    return ((struct array *)st)->len;
}

static void *_rq_top(void *st) {
    return stack_top(st);
}

struct array kd_rangeQuery(struct kd_node_t *p, struct point query_p, double radius) {
    struct traverse_context ctx;
    struct array results = create_array(0, sizeof(struct point));
    struct array nodes = create_array(0, sizeof(struct candidate_node));
    ctx.radius = radius;
    ctx.results = &results;
    ctx.query_hit = _rq_push;
    ctx.nodes = &nodes;
    ctx.nodes_push = _rq_push;
    ctx.nodes_pop = _rq_pop;
    ctx.nodes_size = _rq_size;
    ctx.nodes_top = _rq_top;

    traverse(p, query_p, &ctx);
    destroy_array(&nodes);
    return *(struct array *)ctx.results;
}

/* ------------------------------------------------------- */
/* ------------------------------------------------------- */


/* --------------- METHODS FOR kNN QUERY --------------- */



struct array kd_kNNQuery(struct kd_node_t *p, struct point query_p, int K) {

    return create_array(0, sizeof(struct point));
}

/* ----------------------------------------------------- */
/* ----------------------------------------------------- */
