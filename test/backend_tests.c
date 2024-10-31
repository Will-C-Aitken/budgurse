#include "test.h"


int backend_tests() {
    mu_run_test(entry_to_sql_insert_test);
    mu_run_test(load_empty_db_test);
    mu_run_test(write_entry_test);
    mu_run_test(load_db_test);
    mu_run_test(cat_to_sql_insert_test);
    mu_run_test(del_entry_to_sql_test);
    mu_run_test(edit_entry_to_sql_update_test);
    return 0;
}


int entry_to_sql_insert_test() {
    int cat_id = 1;

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date, -12.00, cat_id, 
	    "this was fun");

    char* sql = entry_to_sql_insert(test_entry1);
    char* expected_statement = "INSERT INTO Entries (name, date, amount, " 
        		       "category_id, note) VALUES "
        		       "('Starbucks', 1644642000, -12.00, 1, "
        		       "'this was fun');"; 

    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 1);
    free_entry(test_entry1);
    free(sql);
    return 0;
}


int load_empty_db_test() {

    init_db("data/test.db");
    g_entries = init_entry_list();
    g_categories = init_cat_array();
    load_db();
    mu_assert(g_entries->num_nodes == 0, "Backend", 2);
    mu_assert(g_categories->num_cats == 0, "Backend", 3);

    free_entry_list(g_entries);
    free_cat_array(g_categories);
    int result = sqlite3_close(g_db);
    mu_assert(result == SQLITE_OK, "Backend", 4);

    return 0;
}


int write_entry_test() {
    // First write normal entry, then entry with NULL note
    int cat_id = 1;
    
    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* e1 = init_entry(1, "Starbucks", test_date, -12.00, cat_id, 
	    "this was fun");

    init_db("data/test.db");

    int result = db_exec(e1, (gen_sql_fn_t)entry_to_sql_insert);
    mu_assert(result == 0, "Backend", 5);

    // NULL note 
    entry_t* e2 = init_entry(2, "Tim Horton's", test_date, -11.00, cat_id, 
            NULL);
    
    result = db_exec(e2, (gen_sql_fn_t)entry_to_sql_insert);
    mu_assert(result == 0, "Backend", 6);

    result = sqlite3_close(g_db);
    mu_assert(result == SQLITE_OK, "Backend", 7);
    free_entry(e1);
    free_entry(e2);
    return 0;
}


int load_db_test() {
    // assumes the tests from prior fn have been executed
    
    init_db("data/test.db");
    g_entries = init_entry_list();
    g_categories = init_cat_array();
    load_db();
    mu_assert(g_entries->num_nodes == 2, "Backend", 8);

    // check tail
    entry_t* tail = g_entries->tail->data;
    mu_assert(strcmp(tail->name, "Tim Horton's") == 0, "Backend", 9);
    mu_assert(tail->amount == -11.00, "Backend", 10);

    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_time1 = mktime(&test_time_tm1);
    mu_assert(difftime(test_time1, tail->date) == 0.0, "Backend", 11);

    free_entry_list(g_entries);
    free_cat_array(g_categories);

    int result = sqlite3_close(g_db);
    mu_assert(result == SQLITE_OK, "Backend", 12);

    return 0;
}


int cat_to_sql_insert_test() {
    
    category_t *c = init_category(1, 0, "Food");

    char* sql = cat_to_sql_insert(c);
    char* expected_statement = "INSERT INTO Categories (id, parent_id, name) "
        		       "VALUES (1, 0, 'Food');";

    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 13);
    free_category(c);
    free(sql);

    return 0;
}


int del_entry_to_sql_test() {
    entry_t *e = test_dummy_entry(1);
    char *sql = del_entry_to_sql(e);
    char *expected_statement = "DELETE FROM Entries WHERE id=1;";
    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 14);
    free_entry(e);
    free(sql);
    return 0;
}


int edit_entry_to_sql_update_test() {
    int cat_id = 1;
    int id = 1;
    struct tm tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t d1 = mktime(&tm1);
    entry_t *e = init_entry(id, "Starbucks", d1, -0.21, cat_id, NULL);

    char *sql = edit_entry_to_sql_update(e);
    char* expected_statement = "UPDATE Entries SET name = 'Starbucks', "
			       "date = 1644642000, amount = -0.21, "
        		       "category_id = 1, note = NULL WHERE"
        		       " id=1;"; 

    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 15);
    free_entry(e);
    free(sql);
    return 0;
}
