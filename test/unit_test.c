#include "test.h"

int tests_run = 0;


char* all_tests() {
    mu_run_test(append_to_tail_001);
    mu_run_test(free_tail_002);
    mu_run_test(free_entry_list_003);
    mu_run_test(entry_to_sql_insert_004);
    mu_run_test(load_empty_db_005);
    mu_run_test(write_entry_006);
    mu_run_test(load_db_007);
    mu_run_test(where_in_list_008);
    mu_run_test(browser_init_009);
    mu_run_test(is_end_node_010);
    mu_run_test(entry_node_traverse_011);
    mu_run_test(browser_scroll_012);
    return 0;
}


char* append_to_tail_001() {
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
    append_to_tail(test_list, test_entry_node1);

    // there should be no next or prev
    mu_assert("Failure in 001-1", !test_list->head->prev);
    mu_assert("Failure in 001-2", !test_list->head->next);
    mu_assert("Failure in 001-3", !test_list->tail->next);
    mu_assert("Failure in 001-4", !test_list->tail->prev);
    mu_assert("Failure in 001-4", test_list->num_nodes == 1);

    // head and tail are same
    mu_assert("Failure in 001-5", test_list->tail == test_entry_node1);
    mu_assert("Failure in 001-6", test_list->head == test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(test_list, test_entry_node2);

    mu_assert("Failure in 001-7", !test_list->head->prev);
    mu_assert("Failure in 001-8", test_list->head->next == test_list->tail);
    mu_assert("Failure in 001-9", !test_list->tail->next);
    mu_assert("Failure in 001-10", test_list->tail->prev == test_list->head);

    mu_assert("Failure in 001-11", test_list->head == test_entry_node1);
    mu_assert("Failure in 001-12", test_list->tail == test_entry_node2);

    free_entry_list(test_list);

    return 0;
}
 

char* free_tail_002() {
    entry_list_t* test_list = init_entry_list();

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(test_list, test_entry_node2);

    free_tail(test_list);
    mu_assert("Failure in 002-01", test_list->num_nodes == 1);
    mu_assert("Failure in 002-02", !test_list->tail->next);
    mu_assert("Failure in 002-03", !test_list->tail->prev);
    mu_assert("Failure in 002-04", !test_list->head->next);
    mu_assert("Failure in 002-05", !test_list->head->prev);
    mu_assert("Failure in 002-06", test_list->head == test_list->tail);

    free_tail(test_list);
    mu_assert("Failure in 002-07", test_list->num_nodes == 0);
    mu_assert("Failure in 002-08", !test_list->head);
    mu_assert("Failure in 002-09", !test_list->tail);

    free_tail(test_list);
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
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Cafe", "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(test_list, test_entry_node2);

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

    // check tail
    entry_t* tail = entries->tail->data;
    mu_assert("Failure in 007-04", strcmp(tail->name, "Tim Horton's") == 0);
    mu_assert("Failure in 007-05", tail->amount == -11.00);

    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_time1 = mktime(&test_time_tm1);
    mu_assert("Failure in 007-06", difftime(test_time1, tail->date) == 0.0);

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
    append_to_tail(entries, test_entry_node1);

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
    append_to_tail(entries, test_entry_node2);

    // test_entry1 is still in list 
    result = where_in_list(entries, test_entry1);
    mu_assert("Failure in 008-4", result == 0);

    // test_entry2 is now in list at position 1
    result = where_in_list(entries, test_entry2);
    mu_assert("Failure in 008-5", result == 1);

    free_entry_list(entries);

    return 0;
}


char *browser_init_009() {
    
    // NULL el passed
    browser_t *b = init_browser(NULL, 10);
    mu_assert("Failure in 009-1", !b->start);
    mu_assert("Failure in 009-2", !b->sel);
    mu_assert("Failure in 009-3", !b->end);
    free_browser(b);

    // empty el passed
    entry_list_t *el = init_entry_list();
    b = init_browser(el, 10);
    mu_assert("Failure in 009-4", !b->start);
    mu_assert("Failure in 009-5", !b->sel);
    mu_assert("Failure in 009-6", !b->end);
    free_browser(b);

    // list with one item but less than 5 rows in window (too small vertically)
    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date1, -20.99, "Food",
	    "Cafe", "Zionist scum!");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(el, test_entry_node1);

    b = init_browser(el, 4);
    mu_assert("Failure in 009-7", !b->start);
    mu_assert("Failure in 009-8", !b->sel);
    mu_assert("Failure in 009-9", !b->end);
    free_browser(b);

    // list with one item and enough space for it
    b = init_browser(el, 5);
    mu_assert("Failure in 009-10", b->start = el->tail);
    mu_assert("Failure in 009-11", b->sel = el->tail);
    mu_assert("Failure in 009-12", b->end = el->tail);
    free_browser(b);

    // two items but only room for 1
    struct tm test_time_tm2 = {.tm_mday=2, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date2 = mktime(&test_time_tm2);
    entry_t* test_entry2 = init_entry("Metro", test_date2, -20.99, "Food",
            "Groceries", NULL);
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(el, test_entry_node2);
    b = init_browser(el, 5);
    mu_assert("Failure in 009-13", b->start = el->tail);
    mu_assert("Failure in 009-14", b->sel = el->tail);
    mu_assert("Failure in 009-15", b->end = el->tail);
    free_browser(b);
    
    // two items with room for both 
    b = init_browser(el, 6);
    mu_assert("Failure in 009-16", b->start = el->head);
    mu_assert("Failure in 009-17", b->sel = el->tail);
    mu_assert("Failure in 009-18", b->end = el->tail);
    free_browser(b);

    free_entry_list(el);

    return 0;
}




char *is_end_node_010() {
    entry_list_t* el = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry("Starbucks", d1, -0.21, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(el, en1);

    mu_assert("Failure in 010-1", is_end_node(en1));

    entry_t* e2 = init_entry("Starbucks", d1, -0.21, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(el, en2);

    // Both are still end nodes
    mu_assert("Failure in 010-2", is_end_node(en2));
    mu_assert("Failure in 010-3", is_end_node(en1));

    entry_t* e3 = init_entry("Starbucks", d1, -0.31, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(el, en3);

    // 1 and 3 are end nodes but 2 isn't
    mu_assert("Failure in 010-4", is_end_node(en1));
    mu_assert("Failure in 010-5", is_end_node(en3));
    mu_assert("Failure in 010-6", !is_end_node(en2));
    mu_assert("Failure in 010-7", is_head(en1));
    mu_assert("Failure in 010-8", is_tail(en3));

    free_entry_list(el);
    return 0;
}


char *entry_node_traverse_011() {
    entry_list_t* el = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry("Starbucks", d1, -0.21, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(el, en1);

    entry_t* e2 = init_entry("Starbucks", d1, -0.21, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(el, en2);

    entry_t* e3 = init_entry("Starbucks", d1, -0.31, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(el, en3);

    entry_node_t* temp = el->tail;

    // temp should stay at node 3
    entry_node_traverse(&temp, DOWN);
    mu_assert("Failure in 011-1", temp == en3);

    entry_node_traverse(&temp, UP);
    mu_assert("Failure in 011-2", temp == en2);

    entry_node_traverse(&temp, UP);
    mu_assert("Failure in 011-3", temp == en1);

    // temp should stay at node 1
    entry_node_traverse(&temp, UP);
    mu_assert("Failure in 011-4", temp == en1);

    free_entry_list(el);
    return 0;
}


char *browser_scroll_012() {
    entry_list_t* el = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry("Starbucks", d1, -0.21, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(el, en1);

    entry_t* e2 = init_entry("Starbucks", d1, -0.21, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(el, en2);

    entry_t* e3 = init_entry("Starbucks", d1, -0.31, "Food",
	NULL, "Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(el, en3);

    // browser with room for full list
    browser_t *b = init_browser(el, 10);
    
    // can't scroll down
    browser_scroll(b, 1, DOWN); 
    mu_assert("Failure in 012-1", b->start == en1);
    mu_assert("Failure in 012-2", b->sel == en3);
    mu_assert("Failure in 012-3", b->end == en3);

    browser_scroll(b, 1, UP); 
    mu_assert("Failure in 012-4", b->start == en1);
    mu_assert("Failure in 012-5", b->sel == en2);
    mu_assert("Failure in 012-6", b->end == en3);

    // scroll beyond head
    browser_scroll(b, 2, UP); 
    mu_assert("Failure in 012-7", b->start == en1);
    mu_assert("Failure in 012-8", b->sel == en1);
    mu_assert("Failure in 012-9", b->end == en3);

    browser_scroll(b, 2, DOWN); 
    mu_assert("Failure in 012-10", b->start == en1);
    mu_assert("Failure in 012-11", b->sel == en3);
    mu_assert("Failure in 012-12", b->end == en3);

    free_browser(b);

    // only room for bottom 2 in el
    b = init_browser(el, 6);

    browser_scroll(b, 1, UP); 
    mu_assert("Failure in 012-13", b->start == en2);
    mu_assert("Failure in 012-14", b->sel == en2);
    mu_assert("Failure in 012-15", b->end == en3);

    // one more changes start end context
    browser_scroll(b, 1, UP); 
    mu_assert("Failure in 012-16", b->start == en1);
    mu_assert("Failure in 012-17", b->sel == en1);
    mu_assert("Failure in 012-18", b->end == en2);

    browser_scroll(b, 2, DOWN); 
    mu_assert("Failure in 012-19", b->start == en2);
    mu_assert("Failure in 012-20", b->sel == en3);
    mu_assert("Failure in 012-21", b->end == en3);

    free_entry_list(el);
    free_browser(b);

    return 0;
}
