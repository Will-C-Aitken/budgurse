#include "entries.h"

llist_t *g_entries = NULL;

// ---------------------------------------------------------------------------
// Entry Definitions
// ---------------------------------------------------------------------------

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

int entry_date_comp_gte(entry_t *e1, entry_t *e2) {
    return (e1->date >= e2->date);
}
