#include "entries.h"

entry_t* init_entry(char* name, float amount, char* category, 
    char* subcategory, char* note) {

    entry_t* e = malloc(sizeof(entry_t));

    e->amount = amount;
    e->name = strdup(name);
    e->category = strdup(category);
    e->subcategory = strdup(subcategory);
    e->note = strdup(note);

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


entry_node_t* init_entry_node(entry_t* e) {

    entry_node_t* nd = malloc(sizeof(entry_node_t));

    nd->next = NULL;
    nd->prev = NULL;
    nd->data = e;

    return nd;
    
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


void append_to_tail(const entry_list_t* el, const entry_node_t* nd) {

    el->num_nodes++;
    
    if(el->num_nodes == 0) {
	el->head = nd;
	el->tail = nd;
	return;
    }

    nd->prev = el->tail;
    el->tail = nd;
}


void free_head(const entry_list_t* el) {

    if(el->num_nodes == 0)
	return;
    

    entry_node_t* temp = el->tail;
    el->tail = el->tail->prev;
    free_entry(temp->data);
    free(temp);

    return
}

typedef struct entry {
    char* name;
    float amount;
    char* category;
    char* subcategory;
    char* note;
} entry_t;

void list_to_string(const entry_list_t* el) {

    if(el->head == NULL) {
	printf("Empty List"); 
	return;
    }

    entry_node_t* cur = el->head;

    while(cur->next != NULL) {
	printf("%s %d %s %s %s\n", *cur->data->name, cur->data->amount, 
		*cur->data->category, *cur->data->subcategory, 
		*cur->data->note)
    }

}
