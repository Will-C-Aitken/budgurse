#include "entries.h"

// ---------------------------------------------------------------------------
// Entry Definitions
// ---------------------------------------------------------------------------

entry_t *init_entry(char *name, time_t date, float amount, 
	char* category, char *subcategory, char *note) {

    entry_t *e = malloc(sizeof(entry_t));

    e->date = date;
    e->amount = amount;
    e->name = strdup(name);
    e->category = strdup(category);

    if (subcategory != NULL)
	e->subcategory = strdup(subcategory);
    else
	e->subcategory = NULL;

    if (note != NULL)
	e->note = strdup(note);
    else
	e->note = NULL;

    return e;
}   


void free_entry(entry_t *e) {
    free(e->name);
    free(e->category);
    free(e->subcategory);
    free(e->note);
    free(e);
}

// ---------------------------------------------------------------------------
// Entry Node Definitions
// ---------------------------------------------------------------------------

entry_node_t *init_entry_node(entry_t *e) {
    
    entry_node_t *nd = malloc(sizeof(entry_node_t));
    
    nd->next = NULL;
    nd->data = e;

    return nd;
}


void free_entry_node(entry_node_t *en) {
    free_entry(en->data);
    free(en);
}

// ---------------------------------------------------------------------------
// Entry List Definitions
// ---------------------------------------------------------------------------

entry_list_t *init_entry_list() {

    entry_list_t *el = malloc(sizeof(entry_list_t));

    el->head = NULL;
    el->num_nodes = 0;
    
    return el;
}


void entry_list_push(entry_list_t *el, entry_node_t *en) {

    el->num_nodes++;
    
    if(el->num_nodes == 1) {
	el->head = en;
	return;
    }

    en->next = el->head;
    el->head = en;
}


entry_node_t *entry_list_pop(entry_list_t *el) {

    entry_node_t *popped_entry_node = NULL;
    if(el->num_nodes == 0)
	return popped_entry_node;

    popped_entry_node = el->head;
    el->head = popped_entry_node->next;
    el->num_nodes--;
    
    return popped_entry_node;
}


void free_entry_list(entry_list_t *el) {

    entry_node_t* head = entry_list_pop(el);
    while(head) {
	free_entry_node(head);
	head = entry_list_pop(el);
    }

    free(el);
    return;
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
