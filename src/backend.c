#include "backend.h"

int init_callback(void *NotUsed, int argc, char **argv, char **azColName) {

    printf("Hello: %d\n", argc);
    for (int i = 0; i < argc; i++) {
	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    return 0;

}


int init_db(sqlite3** db) {

    char* err_msg;
    mkdir("data/", 0777);
    int rc = sqlite3_open("data/budgurse.db", db);
    
    if (rc) {
	printf("Can't open database: %s\n", sqlite3_errmsg(*db));
	return 1;
    }

    char* sql = "CREATE TABLE IF NOT EXISTS entries(id INTEGER PRIMARY KEY, \
		 name TEXT NOT NULL, amount REAL NOT NULL, \
		 category TEXT NOT NULL, subcategory TEXT, note TEXT);";

    rc = sqlite3_exec(*db, sql, init_callback, 0, &err_msg);
    return rc;
}


void load_db(sqlite3* db, entry_list_t* el) {
    return;
}

