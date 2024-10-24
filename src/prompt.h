#ifndef PROMPT_H
#define PROMPT_H

#include "budgurse.h"

// Processing function takes a string as input, validates it, then, if valid, 
// converts it to the correct type and writes it to the second input pointer
typedef int (*input_proc_fn_t) (char *, void *);

int prompt_new_entry();
int prompt_add_category(const char *cat_name, int parent_id);
void prompt_edit_entry(entry_node_t *cur);

int get_entry_input(const char *prompt_str, void *output, 
	input_proc_fn_t p_fn);
void display_prompt(const char* prompt_str, int line, bool refresh);
int get_prompt_response(char* pr);

int date_proc(char *buf, time_t *date);
int name_proc(char *buf, char *name);
int amount_proc(char *buf, float *amount);
int m_cat_proc(char *buf, int *id);
int s_cat_proc(char *buf, int *id);
int cat_proc(char *buf, int *id, bool is_main_cat);
int note_proc(char *buf, char *note);

int check_time_bounds(int day, int month, int year);
// int scan_cat(const char *buf, char* cat_name, char* subcat_name);
// void wscan_str(WINDOW* win, char *buf, unsigned len);
#endif
