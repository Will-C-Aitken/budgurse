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
