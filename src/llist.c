#include "llist.h"

// ---------------------------------------------------------------------------
// Llist Node Definitions
// ---------------------------------------------------------------------------

llist_node_t *init_llist_node(void *data) {
    
    llist_node_t *nd = malloc(sizeof(llist_node_t));
    
    nd->next = NULL;
    nd->prev = NULL;
    nd->data = data;

    return nd;
}

void llist_node_traverse(llist_node_t **curr, llist_dir_t dir) {
    if (dir == DOWN && (*curr)->next) *curr = (*curr)->next;
    if (dir == UP && (*curr)->prev) *curr = (*curr)->prev;
}

int llist_is_end_node(const llist_node_t* curr) {
    if (curr == NULL || llist_is_head(curr) || llist_is_tail(curr)) return 1;
    return 0;
}

int llist_is_head (const llist_node_t* curr) {
    if (!curr->prev) return 1;
    return 0;
}

int llist_is_tail (const llist_node_t* curr) {
    if (!curr->next) return 1;
    return 0;
}

int llist_dist_between(llist_node_t* nd1, const llist_node_t* nd2) {
    int count = 0;
    llist_node_t *temp = nd1;

    // go forward first
    while(temp) {
	if (temp == nd2)
	    return count;
	temp = temp->next;
	count++;
    }
    
    // try backward
    temp = nd1;
    count = 0;
    while(temp) {
	if (temp == nd2)
	    return count;
	temp = temp->prev;
	count++;
    }

    // not in same list, return -1
    return -1;
}

void free_llist_node(llist_node_t *nd, llist_free_data_fn_t f_fn) {
    f_fn(nd->data);
    free(nd);
}

// ---------------------------------------------------------------------------
// Llist Definitions
// ---------------------------------------------------------------------------

llist_t *init_llist() {

    llist_t *l = malloc(sizeof(llist_t));

    l->head = NULL;
    l->tail = NULL;
    l->num_nodes = 0;
    
    return l;
}

void llist_insert_node(llist_t *l, llist_node_t *nd, llist_comp_fn_t c_fn) {
    
    if (l->num_nodes == 0) {
	l->num_nodes++;
        l->tail = nd;
        l->head = nd;
	return;
    }

    if (c_fn(nd->data, l->tail->data)) {
	llist_insert_to_tail(l, nd);
	return;
    }

    if (c_fn(l->head->data, nd->data)) {
	llist_insert_to_head(l, nd);
	return;
    }

    llist_node_t *temp = l->tail;
    for (int i = l->num_nodes; i > 1; i--) {
	llist_node_traverse(&temp, UP);
	if (c_fn(nd->data, temp->data)) {
	    l->num_nodes++;
	    llist_insert_after_node(temp, nd);
	    return;
	}
	// found node, but do insert inside c_fn (to support ll within ll)
	if (c_fn(temp->data, nd->data) < 1)
	    return;
    }

}

void llist_insert_to_tail(llist_t *l, llist_node_t *nd) {
    l->num_nodes++;
    if (l->num_nodes == 1) {
        l->tail = nd;
        l->head = nd;
	return;
    }

    nd->prev = l->tail;
    l->tail->next = nd;
    l->tail = nd;
}


void llist_insert_to_head(llist_t *l, llist_node_t *nd) {
    l->num_nodes++;
    if (l->num_nodes == 1) {
        l->tail = nd;
        l->head = nd;
	return;
    }

    nd->next = l->head;
    l->head->prev = nd;
    l->head = nd;
}


void llist_insert_after_node(llist_node_t *prev_nd, llist_node_t *nd) {
    nd->next = prev_nd->next;
    nd->prev = prev_nd;
    prev_nd->next->prev = nd;
    prev_nd->next = nd;
}


void llist_del_tail(llist_t *l, llist_free_data_fn_t f_fn) {

    if (l->num_nodes == 0)
        return;

    llist_node_t *temp = l->tail;
    l->tail = temp->prev;
    l->num_nodes--;
    
    free_llist_node(temp, f_fn);

    if (l->num_nodes == 0) {
	l->head = NULL;
        return;
    }

    l->tail->next = NULL;
}

void llist_del_head(llist_t *l, llist_free_data_fn_t f_fn) {
    if (l->num_nodes == 0)
	return;

    llist_node_t *temp = l->head;
    l->head = temp->next;
    l->num_nodes--;

    free_llist_node(temp, f_fn);

    if (l->num_nodes == 0) {
	l->tail = NULL;
        return;
    }

    l->head->prev = NULL;
}

void llist_del_node(llist_t *l, llist_node_t *nd, llist_free_data_fn_t f_fn) {

    if (nd == l->tail) {
	llist_del_tail(l, f_fn);
	return;
    }

    if (nd == l->head) {
	llist_del_head(l, f_fn);
	return;
    }

    nd->prev->next = nd->next;
    nd->next->prev = nd->prev;

    free_llist_node(nd, f_fn);
    l->num_nodes--;
}


void free_llist(llist_t *l, llist_free_data_fn_t f_fn) {
    if (!l)
	return;

    while(l->num_nodes > 0)
	llist_del_tail(l, f_fn);
    free(l);
}
