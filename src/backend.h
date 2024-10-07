#ifndef BACKEND_H
#define BACKEND_H

#include "budgurse.h"

extern sqlite3 *g_db;

void load_db();
void init_db(const char *file_name);

int write_entry(entry_t *e);
char *entry_to_sql_insert(entry_t *e);
void append_to_sql(char **cur_sql, const char *sql_to_append, 
	const char *data_to_append, bool data_is_str_type);

#endif

