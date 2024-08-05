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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct entry {
    char *name;
    time_t date;
    float amount;
    char *category;
    char *subcategory;
    char *note;
    struct entry *next;
    struct entry *prev;
} entry_t;

// doubly linked list for holding entries
typedef struct entry_list {
    entry_t *head;
    entry_t *tail;
    int num_nodes;
} entry_list_t;


// entry declarations
entry_t *init_entry(char *name, time_t date, float amount, 
	char *category, char *subcategory, char *note);
void free_entry(entry_t *e);

//
// entry list declarations
entry_list_t *init_entry_list();
void append_to_tail(entry_list_t *el, entry_t *e);
void free_head(entry_list_t *el);
void free_list(entry_list_t *el);
void list_to_string(const entry_list_t *el);
entry_t *get_tail(entry_list_t *el);

#endif
