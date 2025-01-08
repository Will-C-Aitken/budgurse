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

#include "backend.h"

sqlite3 *g_db = NULL;

static int load_categories_callback(void *_, int argc, char **argv, 
	char **azColName) {

    category_t *c = init_category(strtol(argv[0], NULL, 10),
	strtol(argv[1], NULL, 10), argv[2]);

    llist_node_t *nd = init_llist_node(c);
    llist_insert_node(g_categories, nd, (llist_comp_fn_t)cat_comp);
    return 0;
}


static int load_entries_callback(void *_, int argc, char **argv, 
	char **azColName) {

    entry_t *entry = init_entry(strtol(argv[0], NULL, 10), argv[1], 
	(time_t)strtol(argv[2], NULL, 10), strtof(argv[3], NULL), 
	cat_get_from_id(g_categories, strtol(argv[4], NULL, 10)), argv[5]);

    llist_node_t *nd = init_llist_node(entry);
    llist_insert_to_tail(g_entries, nd);
    return 0;
}


void init_data_path(char **db_path) {
    EXIT_IF(*db_path, "Custom db_path not supported, pass NULL ptr instead");

    const char *db_file = "budgurse.db";
    char *ddir_path = malloc(sizeof(char) * (BUFSIZ-strlen(db_file)));
    *db_path = malloc(sizeof(char) * BUFSIZ);

    const char *home_dir = getenv("HOME");
    if (home_dir)
	snprintf(ddir_path, BUFSIZ-strlen(db_file), "%s/.budgurse/", home_dir);
    else
	EXIT("HOME environment variable not set");

    errno = 0;
    
    if (mkdir(ddir_path, 0777) && errno != EEXIST)
	EXIT("failed to make data directory %s with error: %s", 
	    ddir_path, strerror(errno));
    
    snprintf(*db_path, BUFSIZ, "%s%s",ddir_path, db_file);
    free(ddir_path);
}


void init_db(const char *file_name) {

    char *err_msg;
    int rc = sqlite3_open(file_name, &g_db);
    
    EXIT_IF(rc, "Failed to open database with error message: %s\n", 
	    sqlite3_errstr(rc));
    
    char sql[BUFSIZ]; 
    snprintf(sql, BUFSIZ, 
	"CREATE TABLE IF NOT EXISTS Entries( "
	    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
	    "name VARCHAR(%d) NOT NULL, "
	    "date INTEGER NOT NULL, "
	    "amount REAL NOT NULL, "
	    "category_id INTEGER NOT NULL, "
	    "note VARCHAR(%d), "
	    "FOREIGN KEY (category_id) REFERENCES Categories(id)); "
	"CREATE TABLE IF NOT EXISTS Categories( "
	    "id INTEGER NOT NULL, "
	    "parent_id INTEGER, "
	    "name VARCHAR(%d) NOT NULL, "
	    "PRIMARY KEY (id), "
	    "FOREIGN KEY (parent_id) REFERENCES Categories(id));",
	MAX_NAME_BYTES, MAX_NOTE_BYTES, MAX_CAT_BYTES);

    rc = sqlite3_exec(g_db, sql, 0, 0, &err_msg);

    EXIT_IF(rc, "Failed to initialize database with error message: %s\n", 
	err_msg);
    
    sqlite3_free(err_msg);
}


void load_db() {
    load_cat_table();
    load_entry_table();
}


void load_entry_table() {
    int rc;
    char *err_msg;
    char *sql = "SELECT * FROM Entries ORDER BY date ASC";
    rc = sqlite3_exec(g_db, sql, load_entries_callback, NULL, &err_msg);
    EXIT_IF(rc, "Failed to load entries with error message: %s\n", err_msg);

    sqlite3_free(err_msg);
}


void load_cat_table() {
    int rc;
    char *err_msg;
    char *sql = "SELECT * FROM Categories";
    rc = sqlite3_exec(g_db, sql, load_categories_callback, NULL, &err_msg);
    EXIT_IF(rc, "Failed to load categories with error message: %s\n", 
	err_msg);
    sqlite3_free(err_msg);
}


// takes optional data and a function that generates an sql query which is then
// executed
int db_exec(void *data, gen_sql_fn_t gen_sql) {
    char *err_msg;
    char *sql = gen_sql(data);
    
    int rc = sqlite3_exec(g_db, sql, 0, 0, &err_msg);
    EXIT_IF(rc, "Failed to execute sql command with error message:\n%s\n", 
	err_msg);

    sqlite3_free(err_msg);
    free(sql);

    return rc;
}

char *entry_to_sql_insert(entry_t *e) {

    char *sql_to_append = "INSERT INTO Entries (name, date, amount, "
	"category_id, note) VALUES (";

    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);


    append_to_sql(&sql, NULL, e->name, 1);

    sql_to_append = ", ";

    char date_str[11];
    sprintf(date_str, "%ld", e->date);
    append_to_sql(&sql, sql_to_append, date_str, 0);

    char amount_str[9];
    sprintf(amount_str, "%0.2f", e->amount);
    append_to_sql(&sql, sql_to_append, amount_str, 0);

    char category_id_str[3];
    sprintf(category_id_str, "%d", e->cat->id);
    append_to_sql(&sql, sql_to_append, category_id_str, 0);

    append_to_sql(&sql, sql_to_append, e->note, 1);

    sql_to_append = ");";
    append_to_sql(&sql, sql_to_append, "", 0);

    return sql;
}

char *edit_entry_to_sql_update(entry_t *e) {

    char *sql_to_append = "UPDATE Entries SET name = "; 

    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);

    append_to_sql(&sql, NULL, e->name, 1);

    sql_to_append = ", date = ";
    char date_str[11];
    sprintf(date_str, "%ld", e->date);
    append_to_sql(&sql, sql_to_append, date_str, 0);

    sql_to_append = ", amount = ";
    char amount_str[9];
    sprintf(amount_str, "%0.2f", e->amount);
    append_to_sql(&sql, sql_to_append, amount_str, 0);

    sql_to_append = ", category_id = ";
    char category_id_str[3];
    sprintf(category_id_str, "%d", e->cat->id);
    append_to_sql(&sql, sql_to_append, category_id_str, 0);

    sql_to_append = ", note = ";
    append_to_sql(&sql, sql_to_append, e->note, 1);

    sql_to_append = " WHERE id=";
    char id_str[11];
    sprintf(id_str, "%d", e->id);
    append_to_sql(&sql, sql_to_append, id_str, 0);

    sql_to_append = ";";
    append_to_sql(&sql, sql_to_append, "", 0);

    return sql;
}


char *edit_cat_to_sql_update(category_t *c) {

    char *sql_to_append = "UPDATE Categories SET name = "; 

    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);

    append_to_sql(&sql, NULL, c->name, 1);

    sql_to_append = " WHERE id=";
    char id_str[11];
    sprintf(id_str, "%d", c->id);
    append_to_sql(&sql, sql_to_append, id_str, 0);

    sql_to_append = ";";
    append_to_sql(&sql, sql_to_append, "", 0);

    return sql;
}


char *cat_to_sql_insert(category_t *c) {
    char *sql_to_append = "INSERT INTO Categories (parent_id, name) "
	"VALUES (";
    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);

    char category_id_str[10];
    sprintf(category_id_str, "%d", c->p_id);
    append_to_sql(&sql, NULL, category_id_str, 0);

    sql_to_append = ", ";
    append_to_sql(&sql, sql_to_append, c->name, 1);

    sql_to_append = ");";
    append_to_sql(&sql, sql_to_append, "", 0);

    return sql;
}


char *del_entry_to_sql(entry_t *e) {
    char *sql_to_append = "DELETE FROM Entries WHERE id=";

    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);

    char id_str[11];
    sprintf(id_str, "%d", e->id);
    append_to_sql(&sql, NULL, id_str, 0);

    sql_to_append = ";";
    append_to_sql(&sql, sql_to_append, "", 0);

    return sql;
}


char *del_cat_to_sql(category_t *c) {
    char *sql_to_append = "DELETE FROM Categories WHERE id=";

    char *sql = malloc(1 + (sizeof(char) * strlen(sql_to_append)));
    sql[0] = '\0';
    strcat(sql, sql_to_append);

    char id_str[11];
    sprintf(id_str, "%d", c->id);
    append_to_sql(&sql, NULL, id_str, 0);

    sql_to_append = ";";
    append_to_sql(&sql, sql_to_append, "", 0);

    return sql;
}


void append_to_sql(char **cur_sql, const char *sql_to_append, 
	const char *data_to_append, int data_is_str_type) {
    
    char *format_str = (data_is_str_type) ? "%Q" : "%s";
    char *sql_casted_data = sqlite3_mprintf(format_str, data_to_append);
    int sql_len = (sql_to_append == NULL) ? 0 : strlen(sql_to_append);
    int data_len = strlen(sql_casted_data);

    *cur_sql = realloc(*cur_sql, 1 + 
	(sizeof(char) * (sql_len + strlen(*cur_sql) + data_len)));

    if (sql_to_append != NULL)
	strcat(*cur_sql, sql_to_append);
    strcat(*cur_sql, sql_casted_data);
    sqlite3_free(sql_casted_data);

    return;
}
