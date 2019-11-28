#ifndef _KD_TREE_H_
#define _KD_TREE_H_

void rangeQuery(struct kd_node_t *p, double radius);
void kNNquery(struct kd_node_t *p, int K);

#endif
