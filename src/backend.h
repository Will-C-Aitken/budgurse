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

#ifndef BACKEND_H
#define BACKEND_H

#include "entries.h"
#include "categories.h"
#include "status.h"
#include "global.h"
#include "budgurse.h"

extern sqlite3 *g_db;

void load_db();
void load_entry_table();
void load_cat_table();
void init_data_path(char **db_path);
void init_db(const char *file_name);

typedef char *(*gen_sql_fn_t) (void *);
int db_exec(void *data, gen_sql_fn_t gen_sql);

char *entry_to_sql_insert(entry_t *e);
char *cat_to_sql_insert(category_t *c);
char *del_entry_to_sql(entry_t *e);
char *del_cat_to_sql(category_t *c);
char *edit_entry_to_sql_update(entry_t *e);
char *edit_cat_to_sql_update(category_t *c);
void append_to_sql(char **cur_sql, const char *sql_to_append, 
	const char *data_to_append, int data_is_str_type);

#endif
