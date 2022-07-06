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
	fprintf(stderr, "Can't write %s to database: %s\n", 
		sql, sqlite3_errmsg(db));
	return 1;
    }

    sqlite3_free(err_msg);
    free(sql);

    return rc;
}


char* entry_to_sql_insert(entry_t* e) {

    char* sql_to_append = "INSERT INTO entries (name, date, amount, "
			  "category, subcategory, note) "
			  "VALUES (";

    char* sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);


    append_to_sql(&sql, NULL, e->name, true);

    sql_to_append = ", ";

    char date_str[10];
    sprintf(date_str, "%ld", e->date);
    append_to_sql(&sql, sql_to_append, date_str, false);

    char amount_str[10];
    sprintf(amount_str, "%0.2f", e->amount);
    append_to_sql(&sql, sql_to_append, amount_str, false);

    append_to_sql(&sql, sql_to_append, e->category, true);
    append_to_sql(&sql, sql_to_append, e->subcategory, true);
    append_to_sql(&sql, sql_to_append, e->note, true);

    sql_to_append = ");";
    append_to_sql(&sql, sql_to_append, "", false);

    return sql;
}


void append_to_sql(char** cur_sql, const char* sql_to_append, 
	const char* data_to_append, bool data_is_str_type) {
    
    char* format_str;
    if (data_is_str_type)
	format_str = "%Q";
    else
	format_str = "%s";
	

    char* sql_casted_data = sqlite3_mprintf(format_str, data_to_append);

    int sql_len;
    if (sql_to_append == NULL)
	sql_len = 0;
    else
	sql_len = strlen(sql_to_append);

    int data_len = strlen(sql_casted_data);

    *cur_sql = realloc(*cur_sql, 1 + 
	    (sizeof(char) * (sql_len + strlen(*cur_sql) + data_len)));

    if (sql_to_append != NULL)
	strcat(*cur_sql, sql_to_append);
    strcat(*cur_sql, sql_casted_data);
    sqlite3_free(sql_casted_data);

    return;
}
