#ifndef BACKEND_H
#define BACKEND_H

#include <stdio.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "entries.h"

// ----------------------------------------------------------------------------
// db functions
// ----------------------------------------------------------------------------

int load_db(sqlite3 *db, entry_list_t *el);
int init_db(sqlite3 **db, const char *file);

int write_entry(sqlite3 *db, entry_t *e);
char *entry_to_sql_insert(entry_t *e);
void append_to_sql(char **cur_sql, const char *sql_to_append, 
	const char *data_to_append, bool data_is_str_type);

#endif

