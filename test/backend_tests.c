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

#include "test.h"


int backend_tests() {
    mu_run_test(entry_to_sql_insert_test);
    mu_run_test(load_empty_db_test);
    mu_run_test(write_entry_test);
    mu_run_test(load_db_test);
    mu_run_test(cat_to_sql_insert_test);
    mu_run_test(del_entry_to_sql_test);
    mu_run_test(edit_entry_to_sql_update_test);
    remove("test.db");
    return 0;
}


int entry_to_sql_insert_test() {
    category_t *tc = init_category(1, 0, "Food");

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date, -12.00, tc, 
	"this was fun");

    char* sql = entry_to_sql_insert(test_entry1);
    char* expected_statement = "INSERT INTO Entries (name, date, amount, " 
	"category_id, note) VALUES ('Starbucks', 1644642000, -12.00, 1, " 
	"'this was fun');"; 

    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 1);
    free_entry(test_entry1);
    free_category(tc);
    free(sql);
    return 0;
}


int load_empty_db_test() {

    init_db("test.db");
    g_entries = init_llist();
    g_categories = init_llist();
    load_db();
    mu_assert(g_entries->num_nodes == 0, "Backend", 2);
    mu_assert(g_categories->num_nodes == 0, "Backend", 3);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    int result = sqlite3_close(g_db);
    mu_assert(result == SQLITE_OK, "Backend", 4);

    return 0;
}


int write_entry_test() {
    // First write normal entry, then entry with NULL note
    category_t *tc = init_category(1, 0, "Food");
    
    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* e1 = init_entry(1, "Starbucks", test_date, -12.00, tc, 
	"this was fun");

    init_db("test.db");

    int result = db_exec(e1, (gen_sql_fn_t)entry_to_sql_insert);
    mu_assert(result == 0, "Backend", 5);

    // NULL note 
    entry_t* e2 = init_entry(2, "Tim Horton's", test_date, -11.00, tc, NULL);
    
    result = db_exec(e2, (gen_sql_fn_t)entry_to_sql_insert);
    mu_assert(result == 0, "Backend", 6);

    result = sqlite3_close(g_db);
    mu_assert(result == SQLITE_OK, "Backend", 7);
    free_category(tc);
    free_entry(e1);
    free_entry(e2);
    return 0;
}


int load_db_test() {
    // assumes the tests from prior fn have been executed
    
    init_db("test.db");
    g_entries = init_llist();
    g_categories = init_llist();
    load_db();
    mu_assert(g_entries->num_nodes == 2, "Backend", 8);

    // check tail
    entry_t* tail = g_entries->tail->data;
    mu_assert(strcmp(tail->name, "Tim Horton's") == 0, "Backend", 9);
    mu_assert(tail->amount == -11.00, "Backend", 10);

    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_time1 = mktime(&test_time_tm1);
    mu_assert(difftime(test_time1, tail->date) == 0.0, "Backend", 11);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);

    int result = sqlite3_close(g_db);
    mu_assert(result == SQLITE_OK, "Backend", 12);

    return 0;
}


int cat_to_sql_insert_test() {
    
    category_t *c = init_category(1, 0, "Food");

    char* sql = cat_to_sql_insert(c);
    char* expected_statement = "INSERT INTO Categories (parent_id, name) "
	"VALUES (0, 'Food');";

    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 13);
    free_category(c);
    free(sql);

    return 0;
}


int del_entry_to_sql_test() {
    entry_t *e = init_entry(1, "A Name", 0, -12.00, NULL, "A Note");
    char *sql = del_entry_to_sql(e);
    char *expected_statement = "DELETE FROM Entries WHERE id=1;";
    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 14);
    free_entry(e);
    free(sql);
    return 0;
}


int edit_entry_to_sql_update_test() {
    category_t *tc = init_category(1, 0, "Food");
    int id = 1;
    struct tm tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t d1 = mktime(&tm1);
    entry_t *e = init_entry(id, "Starbucks", d1, -0.21, tc, NULL);

    char *sql = edit_entry_to_sql_update(e);
    char* expected_statement = "UPDATE Entries SET name = 'Starbucks', " 
	"date = 1644642000, amount = -0.21, category_id = 1, note = NULL "
	"WHERE id=1;"; 

    mu_assert(strcmp(sql, expected_statement) == 0, "Backend", 15);
    free_entry(e);
    free_category(tc);
    free(sql);
    return 0;

}
