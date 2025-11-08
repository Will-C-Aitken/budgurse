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
    if (strlen(note) < 1)
	e->note = NULL;
    else
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


void entry_view(const entry_t *e) {
    int row = 1, col = 2;
    win_t *vw = init_popup_win();
    box(vw->win, 0, 0);

    // draw date
    mvwaddstr(vw->win, row, col, "Date: ");
    draw_date(vw->win, e->date, 0);
    row++;

    // draw name
    mvwaddstr(vw->win, row, col, "Name: ");
    draw_str(vw->win, e->name, vw->w - 9, "", 0);
    row++;

    // draw amount
    mvwaddstr(vw->win, row, col, "Amount: ");
    draw_amount(vw->win, e->amount, 13, "", 0, 0, 2);
    row++;

    // draw category(s)
    char *cat, *subcat;
    cat_id_to_names(g_categories, e->cat->id, &cat, &subcat);
    mvwaddstr(vw->win, row, col, "Category: ");
    draw_str(vw->win, cat, vw->w - 14, "", 0);
    row++;

    if (subcat) {
	mvwaddstr(vw->win, row, col, "Subcategory: ");
	draw_str(vw->win, subcat, vw->w - 17, "", 0);
	row++;
    }

    // draw note
    if (e->note) {
	mvwaddstr(vw->win, row, col, "Note: ");
	draw_str(vw->win, e->note, vw->w - 10, "", 0);
    }

    wrefresh(vw->win);
    wgetch(vw->win);
    free_popup_win(vw);
}
