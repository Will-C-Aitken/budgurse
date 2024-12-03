/**@file 
 * @brief Monetary entry data structures. 
 * 
 * Entries are the building blocks of budgurse. Each corresponds to an instance
 * of spending or reimbursement. A doubly linked list holds the entries as a
 * list of nodes next and previous pointers
 */

#ifndef ENTRIES_H
#define ENTRIES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "llist.h"
#include "categories.h"

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

#endif
