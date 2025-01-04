#include "categories.h"

llist_t *g_categories = NULL;

category_t *init_category(int id, int p_id, const char* name) {

    category_t *c = malloc(sizeof(category_t));

    c->id = id;
    c->p_id = p_id;
    c->subcats = NULL;
    c->name = strdup(name);
    c->sum_idx = -1;

    return c;
}   


void free_category(category_t *c) {
    if (!c)
	return;
    free_llist(c->subcats, (llist_free_data_fn_t)free_category);
    free(c->name);
    free(c);
}


int cat_comp(llist_node_t *cn1, llist_node_t *cn2, int inverse) {
    category_t *c1 = (category_t *)cn1->data;
    category_t *c2 = (category_t *)cn2->data;

    // inserting to subcategory list
    if (c2->id == c1->p_id) {
	if (!c2->subcats)
	    c2->subcats = init_llist();
	llist_insert_node(c2->subcats, cn1, (llist_comp_fn_t)cat_comp);
	return -1;
    }

    // skip if comparing sub to a main
    else if (c1->p_id != 0 && c2->p_id == 0) {
	return 0;
    }

    // else do alphabetically
    if (inverse)
	return (strcmp(c2->name, c1->name) > 0);
    return (strcmp(c1->name, c2->name) > 0);
}


void cat_id_to_names(const llist_t *cl, int cat_id, char** cat_name, 
	char** subcat_name) {

    if (!cl)
	return;

    *subcat_name = NULL;
    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	*cat_name = t_cat->name;
	if (t_cat->id == cat_id)
	    return;
	if (t_cat->subcats) {
	    llist_node_t *temp2 = t_cat->subcats->head;
	    category_t *t_cat2; 
	    while (temp2) {
		t_cat2 = (category_t *)temp2->data;
		if (t_cat2->id == cat_id) {
		    *subcat_name = t_cat2->name;
		    return;
		}
		temp2 = temp2->next;
	    }
	}
	temp = temp->next;
    }
}


int cat_name_to_id(const llist_t *cl, const char* name, int p_cat_id) {
    if (!cl)
	return 0;

    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	// is a main cat, just check those names
	if (!p_cat_id) {
	    if (strcmp(name, t_cat->name) == 0)
		return t_cat->id;
	}
	else if (p_cat_id == t_cat->id) {
	    if(!t_cat->subcats)
		return 0;
	    temp = t_cat->subcats->head;
	    while (temp) {
		t_cat = (category_t *)temp->data;
		if (strcmp(name, t_cat->name) == 0)
		    return t_cat->id;
		temp = temp->next;
	    }
	    return 0;
	}
	temp = temp->next;
    }
    return 0;
}


// If cat_id is subcat of p_cat_id, return true. Note that all main categories
// are subcategories of p_cat_id 0
int cat_is_sub(const llist_t *cl, int cat_id, int p_cat_id) {
    
    if (!cl)
	return 0;

    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	if (t_cat->id == p_cat_id) {
	    if (!t_cat->subcats)
		return 0;
	    temp = t_cat->subcats->head;
	    while (temp) {
		t_cat = (category_t *)temp->data;
		if (t_cat->id == cat_id)
		    return 1;
		temp = temp->next;
	    }
	    return 0;
	}
	temp = temp->next;
    }

    return 0;
}


void cat_set_sum_idxs(const llist_t *cl, int *next_idx) {

    if (!cl || cl->num_nodes == 0)
	return;
   
    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	t_cat->sum_idx = (*next_idx)++;
	if (t_cat->subcats)
	     cat_set_sum_idxs(t_cat->subcats, next_idx);
	temp = temp->next;
    }
}


category_t *cat_get_from_id(const llist_t *cl, int cat_id) {

    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	if (t_cat->id == cat_id)
	    return t_cat;
	if (t_cat->subcats) {
	    category_t *sub_t_cat = cat_get_from_id(t_cat->subcats, cat_id);
	    if (sub_t_cat)
		return sub_t_cat;
	}
	temp = temp->next;
    }
    return NULL;
}


category_t *cat_get_from_name(const llist_t *cl, const char *name) {
    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	if (strcmp(name, t_cat->name) == 0)
	    return t_cat;
	if (t_cat->subcats) {
	    category_t *sub_t_cat = cat_get_from_name(t_cat->subcats, name);
	    if (sub_t_cat)
		return sub_t_cat;
	}
	temp = temp->next;
    }
    return NULL;
}


void cat_llist_to_array(const llist_t *cl, category_t **ca[], int *next_idx) {
    if (!cl || cl->num_nodes == 0)
	return;
   
    llist_node_t *temp = cl->head;
    category_t *t_cat; 
    while (temp) {
	t_cat = (category_t *)temp->data;
	(*ca)[*next_idx] = t_cat;
	(*next_idx)++;
	if (t_cat->subcats)
	    cat_llist_to_array(t_cat->subcats, ca, next_idx);
	temp = temp->next;
    }
}


int cat_del_from_llist(llist_t *cl, category_t *c) {
    llist_node_t *temp = cl->head;
    category_t *t_cat; 

    while (temp) {
	t_cat = (category_t *)temp->data;
	if (t_cat == c) {
	    llist_del_node(cl, temp, (llist_free_data_fn_t)free_category);
	    return 1;
	}
	if (t_cat->subcats) {
	    int rc = cat_del_from_llist(t_cat->subcats, c);
	    if (t_cat->subcats->num_nodes == 0) {
		free(t_cat->subcats);
		t_cat->subcats = NULL;
	    }
	    if (rc)
		return rc;
	}
	temp = temp->next;
    }
    return 0;
}


void cat_set_name(category_t *c, const char *name) {
    free(c->name);
    c->name = strdup(name);
}
