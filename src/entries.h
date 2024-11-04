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

extern llist_t *g_entries;

typedef struct entry {
    int id;
    char *name;
    time_t date;
    float amount;
    int category_id;
    char *note;
} entry_t;

// entry declarations
entry_t *init_entry(int id, const char *name, time_t date, float amount, 
	int category_id, const char *note);
void free_entry(entry_t *e);
void entry_set_date(entry_t *e, time_t date);
void entry_set_name(entry_t *e, const char *name);
void entry_set_amount(entry_t *e, float amount);
void entry_set_cat_id(entry_t *e, int cat_id);
void entry_set_note(entry_t *e, const char *note);

// for comparing two entries 
int entry_date_comp_gte(entry_t *e1, entry_t *e2);

#endif
