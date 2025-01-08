/* budgurse - budgeting with curses
 *
 * Copyright (c) 2025 W. C. Aitken 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "entries.h"

llist_t *g_entries = NULL;

entry_t *init_entry(int id, const char *name, time_t date, float amount, 
	category_t *c, const char *note) {

    entry_t *e = malloc(sizeof(entry_t));

    e->id = id;
    e->date = date;
    e->amount = amount;
    e->name = strdup(name);
    e->cat = c;

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

void entry_set_cat(entry_t *e, category_t *c) {
    e->cat = c;
}

void entry_set_note(entry_t *e, const char *note) {
    free(e->note);
    e->note = strdup(note);
}


int entry_date_comp_gte(llist_node_t *en1, llist_node_t *en2, int inverse) {
    entry_t *e1 = (entry_t *)en1->data;
    entry_t *e2 = (entry_t *)en2->data;
    if (inverse)
	return (e2->date >= e1->date);
    return (e1->date >= e2->date);
}


int entry_cat_cond_eq(llist_node_t *en, category_t *c) {
    entry_t *e = (entry_t *)en->data;
    return (e->cat == c);
}
