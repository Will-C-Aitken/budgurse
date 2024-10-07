#include "entries.h"
// ---------------------------------------------------------------------------
// Entry Definitions
// ---------------------------------------------------------------------------

entry_list_t *g_entries = NULL;

entry_t *init_entry(char *name, time_t date, float amount, 
	int category_id, char *note) {

    entry_t *e = malloc(sizeof(entry_t));

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
    
    return el;
}


void append_to_tail(entry_list_t *el, entry_node_t *en) {

    el->num_nodes++;
    
    if (el->num_nodes == 1) {
	el->tail = en;
	el->head = en;
	return;
    }

    en->prev = el->tail;
    el->tail->next = en;
    el->tail = en;
}


void free_tail(entry_list_t *el) {

    if (el->num_nodes == 0)
        return;

    entry_node_t *temp = el->tail;
    el->tail = temp->prev;
    el->num_nodes--;
    
    free_entry_node(temp);

    if (el->num_nodes == 0) {
	el->head = NULL;
	el->tail = NULL;
        return;
    }

    el->tail->next = NULL;

    if (el->num_nodes == 1) {
	el->head = el->tail;
	el->head->next = NULL;
	el->tail->prev = NULL;
    }
}


void free_entry_list(entry_list_t *el) {
    if (!el)
	return;

    while(el->num_nodes > 0)
	free_tail(el);
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
