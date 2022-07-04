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

int init_callback(void*, int, char**, char**);

int load_db(sqlite3* db, entry_list_t* el);
int init_db(sqlite3** db);

int write_entry(sqlite3* db, entry_t* e);

#endif

