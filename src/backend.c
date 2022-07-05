#include "backend.h"

int load_callback(void *el, int argc, char **argv, char **azColName) {

    entry_t* entry = init_entry(argv[1], 
				(time_t)strtol(argv[2], NULL, 10),
				strtof(argv[3], NULL), 
				argv[4], 
				argv[5], 
				argv[6]);

    entry_node_t* entry_node = init_entry_node(entry);
    append_to_tail(el, entry_node);

    return 0;
}


int init_db(sqlite3** db, const char* file) {

    char* err_msg;
    mkdir("data/", 0777);
    int rc = sqlite3_open(file, db);
    
    if (rc) {
	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
	return 1;
    }

    char* sql = "CREATE TABLE IF NOT EXISTS ENTRIES( \
		    id INTEGER PRIMARY KEY, \
		    name TEXT NOT NULL, \
		    date INTEGER NOT NULL, \
		    amount REAL NOT NULL, \
		    category TEXT NOT NULL, \
		    subcategory TEXT, \
		    note TEXT);";

    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    sqlite3_free(err_msg);

    return rc;
}


int load_db(sqlite3* db, entry_list_t* el) {

    char* err_msg;
    char* sql = "SELECT * from ENTRIES";

    int rc = sqlite3_exec(db, sql, load_callback, el, &err_msg);
    sqlite3_free(err_msg);

    return rc;
}


int write_entry(sqlite3* db, entry_t* e) {

    char* err_msg;
    char* sql = entry_to_sql_insert(e);
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc) {
	fprintf(stderr, "Can't write to database: %s\n", sqlite3_errmsg(db));
	return 1;
    }

    sqlite3_free(err_msg);
    free(sql);

    return rc;
}
