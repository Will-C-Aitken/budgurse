#include "backend.h"

sqlite3 *g_db = NULL;

static int load_categories_callback(void *_, int argc, char **argv, 
	char **azColName) {

    category_t *c = init_category(strtol(argv[0], NULL, 10), 
				  strtol(argv[1], NULL, 10), 
				  argv[2]);
    append_to_cat_array(g_categories, c);
    return 0;
}


static int load_entries_callback(void *_, int argc, char **argv, 
	char **azColName) {

    entry_t *entry = init_entry(argv[1], 
				(time_t)strtol(argv[2], NULL, 10),
				strtof(argv[3], NULL), 
				strtol(argv[4], NULL, 10), 
				argv[5]);

    entry_node_t *en = init_entry_node(entry);
    append_to_tail(g_entries, en);
    return 0;
}


void init_db(const char *file_name) {

    char *err_msg;
    mkdir("data/", 0777);
    int rc = sqlite3_open(file_name, &g_db);
    
    EXIT_IF(rc, "Failed to open database with error message: %s\n", 
	    sqlite3_errstr(rc));
    
    char *sql = "CREATE TABLE IF NOT EXISTS ENTRIES( "
        	    "id INTEGER NOT NULL, "
        	    "name TEXT NOT NULL, "
        	    "date INTEGER NOT NULL, "
        	    "amount REAL NOT NULL, "
		    "category_id INTEGER NOT NULL, "
        	    "note TEXT, "
		    "PRIMARY KEY (id), "
        	    "FOREIGN KEY (category_id) REFERENCES CATEGORIES(id)); "
		"CREATE TABLE IF NOT EXISTS CATEGORIES( "
		    "id INTEGER NOT NULL, "
		    "parent_id INTEGER, "
		    "name TEXT NOT NULL, "
		    "PRIMARY KEY (id), "
		    "FOREIGN KEY (parent_id) REFERENCES CATEGORIES(id));";

    rc = sqlite3_exec(g_db, sql, 0, 0, &err_msg);

    EXIT_IF(rc, "Failed to initialize database with error message: %s\n", 
	    err_msg);
    
    sqlite3_free(err_msg);
}


void load_db() {
    
    int rc;
    char *err_msg;
    char *sql = "SELECT * from CATEGORIES";
    rc = sqlite3_exec(g_db, sql, load_categories_callback, NULL, &err_msg);
    EXIT_IF(rc, "Failed to load categories with error message: %s\n", 
	    err_msg);

    sql = "SELECT * from ENTRIES";
    rc = sqlite3_exec(g_db, sql, load_entries_callback, NULL, &err_msg);
    EXIT_IF(rc, "Failed to load entries with error message: %s\n", 
	    err_msg);

    sqlite3_free(err_msg);
}


int write_entry(entry_t *e) {

    char *err_msg;
    char *sql = entry_to_sql_insert(e);
    
    int rc = sqlite3_exec(g_db, sql, 0, 0, &err_msg);
    EXIT_IF(rc, "Failed to write entry to database with error message: %s\n", 
	    err_msg);

    sqlite3_free(err_msg);
    free(sql);

    return rc;
}

char *entry_to_sql_insert(entry_t *e) {

    char *sql_to_append = "INSERT INTO entries (name, date, amount, "
			  "category_id, note) "
			  "VALUES (";

    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
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

    char category_id_str[2];
    sprintf(category_id_str, "%d", e->category_id);
    append_to_sql(&sql, sql_to_append, category_id_str, false);

    append_to_sql(&sql, sql_to_append, e->note, true);

    sql_to_append = ");";
    append_to_sql(&sql, sql_to_append, "", false);

    return sql;
}


void append_to_sql(char **cur_sql, const char *sql_to_append, 
	const char *data_to_append, bool data_is_str_type) {
    
    char *format_str;
    if (data_is_str_type)
	format_str = "%Q";
    else
	format_str = "%s";
	

    char *sql_casted_data = sqlite3_mprintf(format_str, data_to_append);

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
