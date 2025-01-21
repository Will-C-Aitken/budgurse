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

#ifndef ENTRIES_H
#define ENTRIES_H

#include "llist.h"
#include "categories.h"
#include "wins.h"
#include "util.h"
#include "global.h"

extern llist_t *g_entries;

typedef struct entry {
    int id;
    char *name;
    time_t date;
    float amount;
    category_t *cat;
    char *note;
} entry_t;

// entry declarations
entry_t *init_entry(int id, const char *name, time_t date, float amount, 
	category_t *c, const char *note);
void free_entry(entry_t *e);
void entry_set_date(entry_t *e, time_t date);
void entry_set_name(entry_t *e, const char *name);
void entry_set_amount(entry_t *e, float amount);
void entry_set_cat(entry_t *e, category_t *c);
void entry_set_note(entry_t *e, const char *note);

// for comparing two entries 
int entry_date_comp_gte(llist_node_t *en1, llist_node_t *en2, int inverse);

// checking if category matches
int entry_cat_cond_eq(llist_node_t *en, category_t *c);

// GUI fn for viewing entry in full form
void entry_view(const entry_t *e);

#endif
