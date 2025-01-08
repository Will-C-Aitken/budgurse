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

#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "llist.h"
#include "global.h"

extern llist_t *g_categories;

typedef struct category {
    int id;
    int p_id;
    llist_t *subcats;
    char *name;
    int sum_idx;
} category_t;

category_t *init_category(int id, int p_id, const char* name);
void free_category(category_t *c);
int cat_comp(llist_node_t *cn1, llist_node_t *cn2, int inverse);

void cat_id_to_names(const llist_t *cl, int cat_id, char** cat_name, 
	char** subcat_name);
int cat_name_to_id(const llist_t *cl, const char* name, int p_cat_id);
int cat_is_sub(const llist_t *cl, int cat_id, int p_cat_id);
void cat_set_sum_idxs(const llist_t *cl, int *next_idx);
category_t *cat_get_from_id(const llist_t *cl, int cat_id);
category_t *cat_get_from_name(const llist_t *cl, const char *name);
void cat_llist_to_array(const llist_t *cl, category_t **ca[], int *next_idx);
int cat_del_from_llist(llist_t *cl, category_t *c);

void cat_set_name(category_t *c, const char *name);

#endif
