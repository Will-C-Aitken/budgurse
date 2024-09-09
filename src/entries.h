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
} entry_t;

typedef struct entry_node {
    struct entry_node *next;
    entry_t *data;
} entry_node_t;

// linked list for holding entries
typedef struct entry_list {
    entry_node_t *head;
    int num_nodes;
} entry_list_t;

// entry declarations
entry_t *init_entry(char *name, time_t date, float amount, 
	char *category, char *subcategory, char *note);
void free_entry(entry_t *e);

// entry node declarations
entry_node_t *init_entry_node(entry_t *e);
void free_entry_node(entry_node_t *e);

// entry list declarations
entry_list_t *init_entry_list();
void entry_list_push(entry_list_t *el, entry_node_t *en);
entry_node_t *entry_list_pop(entry_list_t *el);
void free_entry_list(entry_list_t *el);
int where_in_list(const entry_list_t *el, const entry_t *e);

#endif
