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

typedef struct entry {
    int id;
    char *name;
    time_t date;
    float amount;
    int category_id;
    char *note;
} entry_t;

typedef struct entry_node {
    struct entry_node *next;
    struct entry_node *prev;
    entry_t *data;
} entry_node_t;

typedef enum direction {
    DOWN = 0,
    UP = 1
} direction_t;

// linked list for holding entries
typedef struct entry_list {
    entry_node_t *head;
    entry_node_t *tail;
    int num_nodes;
    int next_free_id;
} entry_list_t;

extern entry_list_t *g_entries;

// entry declarations
entry_t *init_entry(int id, const char *name, time_t date, float amount, 
	int category_id, const char *note);
void free_entry(entry_t *e);
void entry_set_date(entry_t *e, time_t date);
void entry_set_name(entry_t *e, const char *name);
void entry_set_amount(entry_t *e, float amount);
void entry_set_cat_id(entry_t *e, int cat_id);
void entry_set_note(entry_t *e, const char *note);

// entry node declarations
entry_node_t *init_entry_node(entry_t *e);
void entry_node_traverse(entry_node_t **curr, direction_t dir);
bool is_end_node(const entry_node_t* curr);
bool is_head(const entry_node_t* curr);
bool is_tail(const entry_node_t* curr);
void free_entry_node(entry_node_t *e);

// entry list declarations
entry_list_t *init_entry_list();
void append_to_tail(entry_list_t *el, entry_node_t *en);
void del_entry(entry_list_t *el, entry_node_t *en);
void del_tail(entry_list_t *el);
void del_head(entry_list_t *el);
void free_entry_list(entry_list_t *el);
int where_in_list(const entry_list_t *el, const entry_t *e);

#endif
