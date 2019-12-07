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
#include "pqueue.h"
#include "utils.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define COMPARE(a, b) ((a > b) ? a : b)

// 거리함수 정의.
inline static double kd_dist(struct kd_node_t const *a, struct kd_node_t const *b, int dim)
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

/***
 *         _______..______      ___   .___________. __       ___       __          _______  .______   
 *        /       ||   _  \    /   \  |           ||  |     /   \     |  |        |       \ |   _  \  
 *       |   (----`|  |_)  |  /  ^  \ `---|  |----`|  |    /  ^  \    |  |        |  .--.  ||  |_)  | 
 *        \   \    |   ___/  /  /_\  \    |  |     |  |   /  /_\  \   |  |        |  |  |  ||   _  <  
 *    .----)   |   |  |     /  _____  \   |  |     |  |  /  _____  \  |  `----.   |  '--'  ||  |_)  | 
 *    |_______/    | _|    /__/     \__\  |__|     |__| /__/     \__\ |_______|   |_______/ |______/  
 *                                                                                                    
 */

typedef struct traverse_context *_TC;
typedef void (*NoArgMethod)(_TC);
typedef void (*OneArgMethod)(_TC, void *);
typedef void *(*NoArgMethodRPtr)(_TC);
typedef int (*NoArgMethodRInt)(_TC);

struct candidate_node {
    struct kd_node_t *current_node;
    struct rect rect;
    int dim;
};

struct traverse_context {
    double radius;
    int K;
    void *results;
    int nVisits;
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
    ctx->nodes_push(ctx, &cand);

    struct kd_node_t query_kd;
    query_kd.x[0] = query_p.x;
    query_kd.x[1] = query_p.y;

    while(ctx->nodes_size(ctx)) {
        struct candidate_node *cand = (struct candidate_node *)ctx->nodes_top(ctx);
        struct kd_node_t *node = cand->current_node;
        struct rect rect = cand->rect;
        int dim_idx = cand->dim;
        ctx->nodes_pop(ctx);
        double d = kd_dist(node, &query_kd, 2);

        ++ctx->nVisits;

        if(d < ctx->radius) {
            struct point hitp;
            hitp.x = node->x[0];
            hitp.y = node->x[1];
            ctx->query_hit(ctx, &hitp);
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
                ctx->nodes_push(ctx, &new_cand);
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
                ctx->nodes_push(ctx, &new_cand);
            }
        }
    }

    return;
}

/* --------------- METHODS FOR RANGE QUERY --------------- */

static void _rq_query_hit(_TC ctx, void *elem) {
    stack_push(ctx->results, elem);
}

static void _rq_nodes_push(_TC ctx, void *elem) {
    stack_push(ctx->nodes, elem);
}

static void _rq_nodes_pop(_TC ctx) {
    stack_pop(ctx->nodes);
}

static int _rq_nodes_size(_TC ctx) {
    return ((struct array *)ctx->nodes)->len;
}

static void *_rq_nodes_top(_TC ctx) {
    return stack_top(ctx->nodes);
}

struct array kd_rangeQuery(struct kd_node_t *p, struct point query_p, double radius) {
    int dummy;
    return kd_rangeQuery2(p, query_p, radius, &dummy);
}

struct array kd_rangeQuery2(struct kd_node_t *p, struct point query_p, double radius, int *nVisits) {
    struct traverse_context ctx;
    struct array results = create_array(0, sizeof(struct point));
    struct array nodes = create_array(0, sizeof(struct candidate_node));
    ctx.radius = radius;
    ctx.K = INT_MAX;
    ctx.results = &results;
    ctx.query_hit = _rq_query_hit;
    ctx.nVisits = 0;
    ctx.nodes = &nodes;
    ctx.nodes_push = _rq_nodes_push;
    ctx.nodes_pop = _rq_nodes_pop;
    ctx.nodes_size = _rq_nodes_size;
    ctx.nodes_top = _rq_nodes_top;

    traverse(p, query_p, &ctx);
    destroy_array(&nodes);

    *nVisits = ctx.nVisits;
    return *(struct array *)ctx.results;
}

/* ------------------------------------------------------- */
/* ------------------------------------------------------- */


/* --------------- METHODS FOR kNN QUERY --------------- */

static void _kNN_query_hit(_TC ctx, void *elem) {
    struct pqueue *pq = ctx->results;
    struct point *query_p = pq->_aux;
    if(pq_size(pq) < ctx->K) {
        pq_push(pq, elem);
        if(pq_size(pq) == ctx->K) {
            ctx->radius = dist_p(query_p, pq_top(pq));
        }
        return;
    }

    struct point *curFarthest = (struct point *)pq_top(pq);
    if(pq->_cmp(query_p, elem, curFarthest) < 0) {
        pq_pop(pq);
        pq_push(pq, elem);
        ctx->radius = dist_p(query_p, elem);
        return;
    }
}

static void _kNN_nodes_push(_TC ctx, void *elem) {
    pq_push(ctx->nodes, elem);
}

static void _kNN_nodes_pop(_TC ctx) {
    pq_pop(ctx->nodes);
}

static int _kNN_nodes_size(_TC ctx) {
    return pq_size(ctx->nodes);
}

static void *_kNN_nodes_top(_TC ctx) {
    return pq_top(ctx->nodes);
}

static int results_comparator(void *aux, void const *a, void const *b) {
    struct point *point = aux;
    struct point const *lhs = a;
    struct point const *rhs = b;
    // maxheap
    return sgn(dist_p(lhs, point) - dist_p(rhs, point));
}

static int nodes_comparator(void *aux, void const *a, void const *b) {
    struct kd_node_t *point = aux;
    struct candidate_node const *lhs = a;
    struct candidate_node const *rhs = b;
    // minheap
    return sgn(kd_dist(rhs->current_node, point, 2) - kd_dist(lhs->current_node, point, 2));
}

struct array kd_kNNQuery(struct kd_node_t *p, struct point query_p, int K) {
    int dummy;
    return kd_kNNQuery2(p, query_p, K, &dummy);
}

struct array kd_kNNQuery2(struct kd_node_t *p, struct point query_p, int K, int *nVisits) {
    struct kd_node_t query_p_kd;
    query_p_kd.x[0] = query_p.x;
    query_p_kd.x[1] = query_p.y;
    struct pqueue results = create_pq(sizeof(struct point), results_comparator, &query_p);
    struct pqueue nodes = create_pq(sizeof(struct candidate_node), nodes_comparator, &query_p_kd);

    struct traverse_context ctx;
    ctx.radius = INFINITY;
    ctx.K = K;
    ctx.results = &results;
    ctx.query_hit = _kNN_query_hit;
    ctx.nVisits = 0;
    ctx.nodes = &nodes;
    ctx.nodes_push = _kNN_nodes_push;
    ctx.nodes_pop = _kNN_nodes_pop;
    ctx.nodes_size = _kNN_nodes_size;
    ctx.nodes_top = _kNN_nodes_top;

    traverse(p, query_p, &ctx);
    destroy_pq(&nodes);

    struct array ret = pq_to_array(&results);
    destroy_pq(&results);
    *nVisits = ctx.nVisits;
    return ret;
}

/* ----------------------------------------------------- */
/* ----------------------------------------------------- */
