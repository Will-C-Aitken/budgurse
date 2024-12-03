#ifndef PROMPT_H
#define PROMPT_H

#include "budgurse.h"

// Processing function takes a string as input, validates it, then, if valid, 
// converts it to the correct type and writes it to the second input pointer
typedef int (*input_proc_fn_t) (char *, void *);

llist_node_t *prompt_new_entry_node();
int prompt_add_category(const char *cat_name, int parent_id);
void prompt_edit_entry(llist_node_t *cur);
void prompt_edit_category(category_t *c);
int prompt_del_category(category_t *c);

int prompt_for_input(const char *prompt_str, void *output, 
	input_proc_fn_t p_fn);
void prompt_display(const char* prompt_str, int line, int refresh);
int prompt_get_response(char* pr);

int date_proc(char *buf, time_t *date);
int name_proc(char *buf, char *name);
int amount_proc(char *buf, float *amount);
int m_cat_proc(char *buf, int *id);
int s_cat_proc(char *buf, int *id);
int cat_proc(char *buf, int *id, int is_main_cat);
int cat_name_proc(char *buf, char *name);
int note_proc(char *buf, char *note);

#endif
