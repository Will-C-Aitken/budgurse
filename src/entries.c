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

entry_node_t* init_entry_node(
