#include "test.h"

int tests_run = 0;


char* all_tests() {
    mu_run_test(entry_list_push_001);
    mu_run_test(entry_list_pop_002);
    mu_run_test(free_entry_list_003);
    mu_run_test(entry_to_sql_insert_004);
    mu_run_test(load_empty_db_005);
    mu_run_test(write_entry_006);
    mu_run_test(load_db_007);
    mu_run_test(where_in_list_008);
    mu_run_test(browser_buffer_009);
    return 0;
}


char* entry_list_push_001() {
    entry_list_t* test_list = init_entry_list();

    struct tm test_time_tm1 = {.tm_sec = 0,
			       .tm_min = 0,
			       .tm_hour = 0,
			       .tm_mday = 12,
			       .tm_mon = 1, // 0-based so 6 is July
			       .tm_year = 2022 - 1900, // year starts at 1900
			       .tm_isdst = 1}; // daylight savings?

    time_t test_date = mktime(&test_time_tm1);

    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food",
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    entry_list_push(test_list, test_entry_node1);

    entry_t* test_entry2 = test_list->head->data;
    mu_assert("Failure in 001", strcmp(test_entry2->name, "Starbucks") == 0);
    free_entry_list(test_list);

    return 0;
}
 

char* entry_list_pop_002() {
    entry_list_t* test_list = init_entry_list();

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    entry_list_push(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    entry_list_push(test_list, test_entry_node2);

    entry_node_t* popped_node = entry_list_pop(test_list);
    mu_assert("Failure in 002-01", test_list->num_nodes == 1);
    if (popped_node)
	free_entry_node(popped_node);

    popped_node = entry_list_pop(test_list);
    mu_assert("Failure in 002-02", test_list->num_nodes == 0);
    if (popped_node)
	free_entry_node(popped_node);

    popped_node = entry_list_pop(test_list);
    if (popped_node)
	free_entry_node(popped_node);

    free(test_list);

    return 0;

}


char* free_entry_list_003() {
    entry_list_t* test_list = init_entry_list();

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};

    time_t test_date = mktime(&test_time_tm1);

    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    entry_list_push(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    entry_list_push(test_list, test_entry_node2);

    free_entry_list(test_list);

    return 0;

}


char* entry_to_sql_insert_004() {

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};

    time_t test_date = mktime(&test_time_tm1);

    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");

    char* sql = entry_to_sql_insert(test_entry1);
    char* expected_statement = "INSERT INTO entries (name, date, amount, " 
        		       "category, subcategory, note) VALUES "
        		       "('Starbucks', 1644642000, -12.00, 'Food', "
        		       "'Cafe', 'this was fun');"; 

    mu_assert("Failure in 004-01", strcmp(sql, expected_statement) == 0);
    free_entry(test_entry1);
    free(sql);

    return 0;
}


char* load_empty_db_005() {

    sqlite3 *db = NULL;
    int result = init_db(&db, "data/test.db");
    mu_assert("Failure in 005-01", result == 0);
    
    entry_list_t* entries = init_entry_list();
    result = load_db(db, entries);
    mu_assert("Failure in 005-02", result == SQLITE_OK);
    mu_assert("Failure in 005-03", entries->num_nodes == 0);

    free_entry_list(entries);
    result = sqlite3_close(db);
    mu_assert("Failure in 006-04", result == SQLITE_OK);

    return 0;
}


char* write_entry_006() {
    // First write normal entry, then entry with NULL members
    //
    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");

    sqlite3 *db = NULL;
    int result = init_db(&db, "data/test.db");
    mu_assert("Failure in 006-01", result == 0);

    result = write_entry(db, test_entry1);
    mu_assert("Failure in 006-02", result == 0);

    // NULL subcategory and note 
    entry_t* test_entry2 = init_entry("Tim Horton's", test_date, -11.00, "Food", 
            NULL, NULL);
    
    result = write_entry(db, test_entry2);
    mu_assert("Failure in 006-03", result == 0);

    result = sqlite3_close(db);
    mu_assert("Failure in 006-04", result == SQLITE_OK);
    free_entry(test_entry1);
    free_entry(test_entry2);

    return 0;
}


char* load_db_007() {

    // assumes the tests from 006 have already written to db
    sqlite3 *db = NULL;
    int result = init_db(&db, "data/test.db");
    mu_assert("Failure in 007-01", result == 0);
    
    entry_list_t* entries = init_entry_list();
    result = load_db(db, entries);
    mu_assert("Failure in 007-02", result == SQLITE_OK);
    mu_assert("Failure in 007-03", entries->num_nodes == 2);

    // check head
    entry_t* head = entries->head->data;
    mu_assert("Failure in 007-04", strcmp(head->name, "Tim Horton's") == 0);
    mu_assert("Failure in 007-05", head->amount == -11.00);

    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_time1 = mktime(&test_time_tm1);
    mu_assert("Failure in 007-06", difftime(test_time1, head->date) == 0.0);

    free_entry_list(entries);

    result = sqlite3_close(db);
    mu_assert("Failure in 007-10", result == SQLITE_OK);

    return 0;
}


char* where_in_list_008() {

    int result;

    entry_list_t* entries = init_entry_list();
    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date1, -20.99, "Food",
	    "Cafe", "Zionist scum!");

    // empty list, so test_entry1 can't be in it
    result = where_in_list(entries, test_entry1);
    mu_assert("Failure in 008-1", result == -1);

    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    entry_list_push(entries, test_entry_node1);

    // test_entry1 is in list at position 0
    result = where_in_list(entries, test_entry1);
    mu_assert("Failure in 008-2", result == 0);

    struct tm test_time_tm2 = {.tm_mday=2, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date2 = mktime(&test_time_tm2);
    entry_t* test_entry2 = init_entry("Metro", test_date2, -20.99, "Food",
            "Groceries", NULL);
    
    // test_entry2 is not in list
    result = where_in_list(entries, test_entry2);
    mu_assert("Failure in 008-3", result == -1);

    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    entry_list_push(entries, test_entry_node2);

    // test_entry1 is still in list but now at position 1
    result = where_in_list(entries, test_entry1);
    mu_assert("Failure in 008-4", result == 1);

    // test_entry2 is now in list at position 0
    result = where_in_list(entries, test_entry2);
    mu_assert("Failure in 008-5", result == 0);

    free_entry_list(entries);

    return 0;
}


char *browser_buffer_009() {
    
    // NULL el passed
    browser_buf_t *bb = init_browser_buf(NULL, 10);
    mu_assert("Failure in 009-1", !bb->buf_entries);
    mu_assert("Failure in 009-2", bb->buf_len == 0);
    mu_assert("Failure in 009-3", bb->sel_pos == -1);
    free_browser_buf(bb);

    // empty el passed
    entry_list_t *el = init_entry_list();
    bb = init_browser_buf(el, 10);
    mu_assert("Failure in 009-4", !bb->buf_entries);
    mu_assert("Failure in 009-5", bb->buf_len == 0);
    mu_assert("Failure in 009-6", bb->sel_pos == -1);
    free_browser_buf(bb);

    // list with one item but less than 5 rows in window (too small vertically)
    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date1, -20.99, "Food",
	    "Cafe", "Zionist scum!");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    entry_list_push(el, test_entry_node1);
    bb = init_browser_buf(el, 4);
    mu_assert("Failure in 009-7", !bb->buf_entries);
    mu_assert("Failure in 009-8", bb->buf_len == 0);
    mu_assert("Failure in 009-9", bb->sel_pos == -1);
    free_browser_buf(bb);

    // list with one item and enough space for it
    bb = init_browser_buf(el, 6);
    mu_assert("Failure in 009-10", bb->buf_entries[0] == el->head->data);
    mu_assert("Failure in 009-11", bb->buf_len == 1);
    mu_assert("Failure in 009-12", bb->sel_pos == 0);
    free_browser_buf(bb);

    // two items but only room for 1
    struct tm test_time_tm2 = {.tm_mday=2, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date2 = mktime(&test_time_tm2);
    entry_t* test_entry2 = init_entry("Metro", test_date2, -20.99, "Food",
            "Groceries", NULL);
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    entry_list_push(el, test_entry_node2);
    bb = init_browser_buf(el, 5);
    mu_assert("Failure in 009-13", bb->buf_entries[0] == el->head->data);
    mu_assert("Failure in 009-14", bb->buf_len == 1);
    mu_assert("Failure in 009-15", bb->sel_pos == 0);
    free_browser_buf(bb);
    
    // two items with room for both 
    bb = init_browser_buf(el, 6);
    mu_assert("Failure in 009-16", bb->buf_entries[0] == el->head->next->data);
    mu_assert("Failure in 009-17", bb->buf_entries[1] == el->head->data);
    mu_assert("Failure in 009-18", bb->buf_len == 2);
    mu_assert("Failure in 009-19", bb->sel_pos == 1);
    free_browser_buf(bb);

    free_entry_list(el);

    return 0;
}
