#include "categories.h"

cat_array_t *g_categories = NULL;

category_t *init_category(int id, int parent_id, const char* name) {

    category_t *c = malloc(sizeof(category_t));

    c->id = id;
    c->parent_id = parent_id;
    c->name = strdup(name);

    return c;
}   


void free_category(category_t *c) {
    free(c->name);
    free(c);
}


cat_array_t *init_cat_array() {
    cat_array_t *ca = malloc(sizeof(cat_array_t));

    ca->array = NULL;
    ca->num_cats = 0;

    return ca;
}


void free_cat_array(cat_array_t *ca) {
    if (!ca)
	return;
    while (ca->num_cats > 0) {
	free_category(ca->array[ca->num_cats-1]);
	ca->num_cats--;
    }
    free(ca->array);
    free(ca);
}


int append_to_cat_array(cat_array_t *ca, category_t *c) {

    int rc = 0;

    // If parent_id is not NULL, must check that it exists
    if (c->parent_id != 0) {
	rc = 1;

	for (int i = 0; i < ca->num_cats; i++) {
	    // legal parent_id if it is already in array AND if
	    // that parent category is NOT a subcategory itself 
	    if ((ca->array[i]->id == c->parent_id) && 
		(ca->array[i]->parent_id == 0)) {

		rc = 0;
		break;
	    }
	}

	if (rc)
	    return rc;
    }

    ca->num_cats++;
    ca->array = realloc(ca->array, ca->num_cats * sizeof(category_t));
    ca->array[ca->num_cats-1] = c;

    return rc;
}


void cat_id_to_names(const cat_array_t *ca, int cat_id, char** cat_name, 
	char** subcat_name) {
    category_t *cat = get_cat(ca, cat_id);

    // If no subcat, return
    if (cat->parent_id == 0) {
	*cat_name = cat->name;
	*subcat_name = NULL;
	return;
    }

    category_t *subcat = cat;
    cat = get_cat(ca, subcat->parent_id);

    *subcat_name = subcat->name;
    *cat_name = cat->name;
}


category_t* get_cat(const cat_array_t *ca, int cat_id) {
    for (int i = 0; i < ca->num_cats; i++) {
	if (ca->array[i]->id == cat_id)
	    return ca->array[i];
    }
    return NULL;
}


int cat_name_to_id(const cat_array_t *ca, const char* name) {
    for (int i = 0; i < ca->num_cats; i++) {
	if (strcmp(name, ca->array[i]->name) == 0)
	    return ca->array[i]->id;
    }
    return 0;
}
