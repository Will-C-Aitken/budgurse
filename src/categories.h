#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct category {
    int id;
    int parent_id;
    char *name;
} category_t;

category_t *init_category(int id, int parent_id, const char* name);
void free_category(category_t *c);

typedef struct cat_array {
    int num_cats;
    category_t **array;
} cat_array_t;

extern cat_array_t *g_categories;

cat_array_t *init_cat_array();
void free_cat_array(cat_array_t *ca);
int append_to_cat_array(cat_array_t *ca, category_t *c);
void cat_id_to_names(const cat_array_t *ca, int cat_id, char** cat_name, 
	char** subcat_name);
int cat_name_to_id(const cat_array_t *ca, const char* name, bool is_main_cat);
category_t* get_cat(const cat_array_t *ca, int cat_id);
int is_sub_cat(const cat_array_t *ca, int cat_id, int p_cat_id);
int get_next_id(const cat_array_t *ca);

#endif
