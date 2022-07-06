#include "entries.h"

// ---------------------------------------------------------------------------
// Entry Definitions
// ---------------------------------------------------------------------------

entry_t* init_entry(char* name, time_t date, float amount, 
	char* category, char* subcategory, char* note) {

    entry_t* e = malloc(sizeof(entry_t));

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


void free_entry(entry_t* e) {

    free(e->name);
    free(e->category);
    free(e->subcategory);
    free(e->note);
    free(e);
    return;

}


// ---------------------------------------------------------------------------
// Entry Node Definitions
// ---------------------------------------------------------------------------

entry_node_t* init_entry_node(entry_t* e) {

    entry_node_t* nd = malloc(sizeof(entry_node_t));

    nd->next = NULL;
    nd->prev = NULL;
    nd->data = e;

    return nd;
    
}


void free_entry_node(entry_node_t* e) {

    free_entry(e->data);
    free(e);
    return;

}


// ---------------------------------------------------------------------------
// Entry List Definitions
// ---------------------------------------------------------------------------

entry_list_t* init_entry_list() {

    entry_list_t* el = malloc(sizeof(entry_list_t));

    el->head = NULL;
    el->tail = NULL;
    el->num_nodes = 0;
    
    return el;
}


void append_to_tail(entry_list_t* el, entry_node_t* nd) {

    el->num_nodes++;
    
    if(el->num_nodes == 1) {
	el->head = nd;
	el->tail = nd;
	return;
    }

    el->tail->next = nd;
    nd->prev = el->tail;
    el->tail = nd;
}


void free_head(entry_list_t* el) {

    if(el->num_nodes == 0)
	return;


    entry_node_t* temp = el->head;
    el->head = temp->next;
    el->num_nodes--;
    
    free_entry_node(temp);

    if(el->num_nodes == 0)
	return;

    el->head->prev = NULL;
    return;


}


void free_list(entry_list_t* el) {
    while(el->num_nodes > 0)
	free_head(el);
    free(el);
    return;
}



void list_to_string(const entry_list_t* el) {

    if(el->head == NULL) {
	printf("Empty List\n"); 
	return;
    }

    entry_node_t* cur = el->head;

    do {
	printf("%s %ld %0.2f %s %s %s\n", cur->data->name, 
		cur->data->date, cur->data->amount, 
		cur->data->category, cur->data->subcategory, 
		cur->data->note);
	cur = cur->next;
    } while(cur != NULL); 
}


entry_t* get_tail(entry_list_t* el) {
    return(el->tail->data);
}
