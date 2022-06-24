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

void load_db(sqlite3* db, entry_list_t* el);
int init_db(sqlite3** db);


#endif

