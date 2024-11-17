#include "test.h"


llist_t* test_dummy_list(int num_nodes) {
    llist_t *el = init_llist();
    for (int i = 1; i <= num_nodes; i++)
	llist_insert_to_tail(el, init_llist_node(NULL));
    return el;
}


// Add categories alternating between main and sub
llist_t* test_dummy_cat_list(int num_cats) {
    llist_t *ca = init_llist();
    category_t *c = NULL;
    char cat_name[CAT_STR_LEN+1];
    for (int i = 1; i <= num_cats; i++) {
	snprintf(cat_name, CAT_STR_LEN, "Cat%d", i); 
	if (i % 2)
	    c = init_category(i, 0, cat_name);
	else
	    c = init_category(i, i-1, cat_name);
	llist_insert_node(ca, init_llist_node(c), (llist_comp_fn_t)cat_comp);
    }
    return ca;
}


int is_amount_eq(float candidate, float actual) {
    // less than half a cent off is fine
    float e = 0.005;
    return (abs(candidate - actual) < e);
}
