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

#ifndef PROMPT_H
#define PROMPT_H

#include "llist.h"
#include "categories.h"
#include "entries.h"
#include "backend.h"
#include "util.h"
#include "wins.h"
#include "budgurse.h"
#include "global.h"

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
void prompt_default();

int date_proc(char *buf, time_t *date);
int name_proc(char *buf, char *name);
int amount_proc(char *buf, float *amount);
int m_cat_proc(char *buf, int *id);
int s_cat_proc(char *buf, int *id);
int cat_proc(char *buf, int *id, int is_main_cat);
int cat_name_proc(char *buf, char *name);
int note_proc(char *buf, char *note);

#endif
