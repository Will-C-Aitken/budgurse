#ifndef LLIST_H
#define LLIST_H

#include "global.h"

typedef struct llist_node {
    struct llist_node *next;
    struct llist_node *prev;
    void *data;
} llist_node_t;

typedef struct llist {
    llist_node_t *head;
    llist_node_t *tail;
    int num_nodes;
} llist_t;

// For comparing two nodes
typedef int (*llist_comp_fn_t) (llist_node_t *, llist_node_t *, int);
// For checking condition on single node
typedef int (*llist_cond_fn_t) (llist_node_t *, void *);
typedef void (*llist_free_data_fn_t) (void *);

// llist node declarations
llist_node_t *init_llist_node(void *data);
void llist_node_traverse(llist_node_t **curr, dir_t dir);
int llist_is_end_node(const llist_node_t* curr);
int llist_is_head(const llist_node_t* curr);
int llist_is_tail(const llist_node_t* curr);
int llist_dist_between(llist_node_t* en1, const llist_node_t* en2);
void free_llist_node(llist_node_t *nd, llist_free_data_fn_t fn);

// llist declarations
llist_t *init_llist();
int llist_insert_node(llist_t *l, llist_node_t *nd, llist_comp_fn_t c_fn);
void llist_del_node(llist_t *l, llist_node_t *en, llist_free_data_fn_t fn);
void llist_del_tail(llist_t *l, llist_free_data_fn_t fn);
void llist_del_head(llist_t *l, llist_free_data_fn_t fn);
void free_llist(llist_t *l, llist_free_data_fn_t fn);
// free data fn for leaving inner data intact
void free_nop(void *d);

// llist place_fns
void llist_insert_to_tail(llist_t *l, llist_node_t *nd);
void llist_insert_to_head(llist_t *l, llist_node_t *nd);
void llist_insert_after_node(llist_node_t *prev_nd, llist_node_t *nd);

llist_t *llist_get_matches(llist_t *ll, void *data, llist_cond_fn_t c_fn);
llist_node_t *llist_sort_node(llist_t *l, llist_node_t *nd, 
	llist_comp_fn_t c_fn);

#endif
