#ifndef BACKEND_H
#define BACKEND_H

#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "entries.h"
#include "categories.h"
#include "budgurse.h"

extern sqlite3 *g_db;

void load_db();
void init_db(const char *file_name);

typedef char *(*gen_sql_fn_t) (void *);
int db_exec(void *data, gen_sql_fn_t gen_sql);

char *entry_to_sql_insert(entry_t *e);
char *cat_to_sql_insert(category_t *c);
char *del_entry_to_sql(entry_t *e);
char *edit_entry_to_sql_update(entry_t *e);
void append_to_sql(char **cur_sql, const char *sql_to_append, 
	const char *data_to_append, int data_is_str_type);

#endif

