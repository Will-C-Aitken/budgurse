#include "entries.h"
// ---------------------------------------------------------------------------
// Entry Definitions
// ---------------------------------------------------------------------------

entry_list_t *g_entries = NULL;

entry_t *init_entry(int id, const char *name, time_t date, float amount, 
	int category_id, const char *note) {

    entry_t *e = malloc(sizeof(entry_t));

    e->id = id;
    e->date = date;
    e->amount = amount;
    e->name = strdup(name);
    e->category_id = category_id;

    if (note != NULL)
	e->note = strdup(note);
    else
	e->note = NULL;

    return e;
}   


void free_entry(entry_t *e) {
    free(e->name);
    free(e->note);
    free(e);
}

void entry_set_date(entry_t *e, time_t new_date) {
    e->date = new_date;
}

void entry_set_name(entry_t *e, const char *name) {
    free(e->name);
    e->name = strdup(name);
}

void entry_set_amount(entry_t *e, float amount) {
    e->amount = amount;
}

void entry_set_cat_id(entry_t *e, int cat_id) {
    e->category_id = cat_id;
}

void entry_set_note(entry_t *e, const char *note) {
    free(e->note);
    e->note = strdup(note);
}

// ---------------------------------------------------------------------------
// Entry Node Definitions
// ---------------------------------------------------------------------------

entry_node_t *init_entry_node(entry_t *e) {
    
    entry_node_t *nd = malloc(sizeof(entry_node_t));
    
    nd->next = NULL;
    nd->prev = NULL;
    nd->data = e;

    return nd;
}

void entry_node_traverse(entry_node_t **curr, direction_t dir) {
    if (dir == DOWN && (*curr)->next) *curr = (*curr)->next;
    if (dir == UP && (*curr)->prev) *curr = (*curr)->prev;
}


void free_entry_node(entry_node_t *en) {
    free_entry(en->data);
    free(en);
}

bool is_end_node(const entry_node_t* curr) {
    if (curr == NULL || is_head(curr) || is_tail(curr)) return true;
    return false;
}

bool is_head (const entry_node_t* curr) {
    if (!curr->prev) return true;
    return false;
}

bool is_tail (const entry_node_t* curr) {
    if (!curr->next) return true;
    return false;
}

// ---------------------------------------------------------------------------
// Entry List Definitions
// ---------------------------------------------------------------------------

entry_list_t *init_entry_list() {

    entry_list_t *el = malloc(sizeof(entry_list_t));

    el->head = NULL;
    el->tail = NULL;
    el->num_nodes = 0;
    el->next_free_id = 1;
    
    return el;
}


void append_to_tail(entry_list_t *el, entry_node_t *en) {

    el->num_nodes++;
    el->next_free_id = en->data->id + 1;
    
    if (el->num_nodes == 1) {
	el->tail = en;
	el->head = en;
	return;
    }

    en->prev = el->tail;
    el->tail->next = en;
    el->tail = en;
}

void del_tail(entry_list_t *el) {

    if (el->num_nodes == 0)
        return;

    entry_node_t *temp = el->tail;
    el->tail = temp->prev;
    el->num_nodes--;
    
    free_entry_node(temp);

    if (el->num_nodes == 0) {
	el->head = NULL;
        return;
    }

    el->tail->next = NULL;
}

void del_head(entry_list_t *el) {
    if (el->num_nodes == 0)
	return;

    entry_node_t *temp = el->head;
    el->head = temp->next;
    el->num_nodes--;

    free_entry_node(temp);

    if (el->num_nodes == 0) {
	el->tail = NULL;
        return;
    }

    el->head->prev = NULL;
}

void del_entry(entry_list_t *el, entry_node_t *en) {

    if (en == el->tail) {
	del_tail(el);
	return;
    }

    if (en == el->head) {
	del_head(el);
	return;
    }

    en->prev->next = en->next;
    en->next->prev = en->prev;

    free_entry_node(en);
    el->num_nodes--;
}


void free_entry_list(entry_list_t *el) {
    if (!el)
	return;

    while(el->num_nodes > 0)
	del_tail(el);
    free(el);
}


// Return what place entry e is in in el. Return -1 if absent
int where_in_list(const entry_list_t *el, const entry_t *e){

    if (!el->num_nodes)
	return(-1);

    int place = 0;
    entry_node_t *temp = el->head;

    while (place < el->num_nodes) {
        if (temp->data == e)
            return(place);
        place++;
        temp = temp->next;
    }

    return(-1);
}
