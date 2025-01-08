/* budgurse - budgeting with curses
 *
 * Copyright (c) 2025 W. C. Aitken 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

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

void llist_node_traverse(llist_node_t **curr, dir_t dir) {
    if (dir == DOWN && (*curr)->next) 
	*curr = (*curr)->next;
    if (dir == UP && (*curr)->prev) 
	*curr = (*curr)->prev;
}

int llist_is_end_node(const llist_node_t* curr) {
    if (curr == NULL || llist_is_head(curr) || llist_is_tail(curr)) 
	return 1;
    return 0;
}

int llist_is_head (const llist_node_t* curr) {
    if (!curr->prev) 
	return 1;
    return 0;
}

int llist_is_tail (const llist_node_t* curr) {
    if (!curr->next) 
	return 1;
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

int llist_insert_node(llist_t *l, llist_node_t *nd, llist_comp_fn_t c_fn) {
    
    if (l->num_nodes == 0) {
	l->num_nodes++;
        l->tail = nd;
        l->head = nd;
	return 1;
    }

    int res = c_fn(nd, l->tail, 0);
    if (res) {
	if (res > 0)
	    llist_insert_to_tail(l, nd);
	// if negative, insert was already done by c_fn
	return 1;
    }

    // use the inverse version of c_fn because for head we are inserting above
    res = c_fn(nd, l->head, 1);
    if (res) {
	if (res > 0)
	    llist_insert_to_head(l, nd);
	// if negative, insert was already done by c_fn
	return 1;
    }

    llist_node_t *temp = l->tail;
    for (int i = l->num_nodes; i > 1; i--) {
	llist_node_traverse(&temp, UP);
	res = c_fn(nd, temp, 0);
	if (res) {
	    if (res > 0) {
		l->num_nodes++;
		llist_insert_after_node(temp, nd);
	    }
	    // if negative, insert was already done by c_fn
	    return 1;
	}
    }
    
    return 0;

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
    if (!l || !f_fn)
	return;

    while(l->num_nodes > 0)
	llist_del_tail(l, f_fn);
    free(l);
}


llist_t *llist_get_matches(llist_t *ll, void *data, llist_cond_fn_t c_fn) {
    llist_t *matches = init_llist();
    if (!ll || ll->num_nodes == 0)
	return matches;

    llist_node_t *temp = ll->head;
    llist_node_t *matched_ln;
    while (temp) {
	if (c_fn(temp, data)) {
	    matched_ln = init_llist_node(temp->data);
	    llist_insert_to_tail(matches, matched_ln);
	}
	temp = temp->next;
    }
    return matches;
}


llist_node_t *llist_sort_node(llist_t *l, llist_node_t *nd, 
	llist_comp_fn_t c_fn) {
    llist_node_t *n_nd = init_llist_node(nd->data);
    // pop original node without deleting inner data
    llist_del_node(l, nd, (llist_free_data_fn_t)free_nop);
    // now insert back into list
    llist_insert_node(l, n_nd, c_fn);
    return n_nd;
}


void free_nop(void *d) {}
