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
