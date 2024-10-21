#include "test.h"

entry_list_t *g_entries;
cat_array_t *g_categories;
sqlite3 *g_db;
browser_t *g_browser;
win_t wins[NUM_WINS];

int curses_mode = 0;
int tests_run = 0;

int static amount_eq(float candidate, float actual) {
    // less than half a cent off is fine
    float e = 0.005;
    return (abs(candidate - actual) < e);
}

char* all_tests() {
    mu_run_test(append_to_tail_001);
    mu_run_test(del_tail_002);
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
    mu_run_test(cat_array_013);
    mu_run_test(cat_id_to_names_014);
    mu_run_test(date_proc_015);
    mu_run_test(amount_proc_016);
    mu_run_test(cat_proc_017);
    mu_run_test(cat_to_sql_insert_018);
    mu_run_test(del_head_019);
    mu_run_test(del_entry_020);
    mu_run_test(del_entry_to_sql_021);
    mu_run_test(browser_del_entry_022);
    // mu_run_test(edit_entry_to_sql_update_023);
    return 0;
}


char* append_to_tail_001() {
    int category_id = 1;
    entry_list_t* test_list = init_entry_list();

    struct tm test_time_tm1 = {.tm_sec = 0,
			       .tm_min = 0,
			       .tm_hour = 0,
			       .tm_mday = 12,
			       .tm_mon = 1, // 0-based so 6 is July
			       .tm_year = 2022 - 1900, // year starts at 1900
			       .tm_isdst = 1}; // daylight savings?

    time_t test_date = mktime(&test_time_tm1);

    entry_t* test_entry1 = init_entry(0, "Starbucks", test_date, -12.00, 
	    category_id, "this was fun");
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

    entry_t* test_entry2 = init_entry(1, "Tims", test_date, -12.00, category_id, 
	    "this was fun");
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
 

char* del_tail_002() {
    int category_id = 1;
    entry_list_t* test_list = init_entry_list();

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date, -1.00, 
	    category_id, "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry(2, "Tims", test_date, -12.00, category_id, 
	    "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(test_list, test_entry_node2);

    del_tail(test_list);
    mu_assert("Failure in 002-01", test_list->num_nodes == 1);
    mu_assert("Failure in 002-02", !test_list->tail->next);
    mu_assert("Failure in 002-03", !test_list->tail->prev);
    mu_assert("Failure in 002-04", !test_list->head->next);
    mu_assert("Failure in 002-05", !test_list->head->prev);
    mu_assert("Failure in 002-06", test_list->head == test_list->tail);

    del_tail(test_list);
    mu_assert("Failure in 002-07", test_list->num_nodes == 0);
    mu_assert("Failure in 002-08", !test_list->head);
    mu_assert("Failure in 002-09", !test_list->tail);

    del_tail(test_list);
    free(test_list);

    return 0;

}


char* free_entry_list_003() {
    int cat_id = 1;
    entry_list_t* test_list = init_entry_list();

    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};

    time_t test_date = mktime(&test_time_tm1);

    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date, -12.00, cat_id, 
	    "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry(2, "Tims", test_date, -12.00, cat_id, 
	    "this was fun");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(test_list, test_entry_node2);

    free_entry_list(test_list);

    return 0;

}


char* entry_to_sql_insert_004() {
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

    mu_assert("Failure in 004-01", strcmp(sql, expected_statement) == 0);
    free_entry(test_entry1);
    free(sql);

    return 0;
}


char* load_empty_db_005() {

    init_db("data/test.db");
    g_entries = init_entry_list();
    g_categories = init_cat_array();
    load_db();
    mu_assert("Failure in 005-01", g_entries->num_nodes == 0);

    free_entry_list(g_entries);
    free_cat_array(g_categories);
    int result = sqlite3_close(g_db);
    mu_assert("Failure in 005-02", result == SQLITE_OK);

    return 0;
}


char* write_entry_006() {
    // First write normal entry, then entry with NULL members
    int cat_id = 1;
    
    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date, -12.00, cat_id, 
	    "this was fun");

    init_db("data/test.db");

    int result = db_exec(test_entry1, (gen_sql_fn_t)entry_to_sql_insert);
    mu_assert("Failure in 006-01", result == 0);

    // NULL subcategory and note 
    entry_t* test_entry2 = init_entry(2, "Tim Horton's", test_date, -11.00, cat_id, 
            NULL);
    
    result = db_exec(test_entry2, (gen_sql_fn_t)entry_to_sql_insert);
    mu_assert("Failure in 006-02", result == 0);

    result = sqlite3_close(g_db);
    mu_assert("Failure in 006-03", result == SQLITE_OK);
    free_entry(test_entry1);
    free_entry(test_entry2);

    return 0;
}


char* load_db_007() {
    // assumes the tests from 006 have already written to db
    
    init_db("data/test.db");
    g_entries = init_entry_list();
    g_categories = init_cat_array();
    load_db();
    mu_assert("Failure in 007-01", g_entries->num_nodes == 2);

    // check tail
    entry_t* tail = g_entries->tail->data;
    mu_assert("Failure in 007-02", strcmp(tail->name, "Tim Horton's") == 0);
    mu_assert("Failure in 007-03", tail->amount == -11.00);

    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_time1 = mktime(&test_time_tm1);
    mu_assert("Failure in 007-04", difftime(test_time1, tail->date) == 0.0);

    free_entry_list(g_entries);
    free_cat_array(g_categories);

    int result = sqlite3_close(g_db);
    mu_assert("Failure in 007-05", result == SQLITE_OK);

    return 0;
}


char* where_in_list_008() {
    int result;
    int cat_id = 1;

    g_entries = init_entry_list();
    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date1, -20.99, cat_id,
	    "Zionist scum!");

    // empty list, so test_entry1 can't be in it
    result = where_in_list(g_entries, test_entry1);
    mu_assert("Failure in 008-1", result == -1);

    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(g_entries, test_entry_node1);

    // test_entry1 is in list at position 0
    result = where_in_list(g_entries, test_entry1);
    mu_assert("Failure in 008-2", result == 0);

    struct tm test_time_tm2 = {.tm_mday=2, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date2 = mktime(&test_time_tm2);
    entry_t* test_entry2 = init_entry(2, "Metro", test_date2, -20.99, cat_id,
            NULL);
    
    // test_entry2 is not in list
    result = where_in_list(g_entries, test_entry2);
    mu_assert("Failure in 008-3", result == -1);

    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(g_entries, test_entry_node2);

    // test_entry1 is still in list 
    result = where_in_list(g_entries, test_entry1);
    mu_assert("Failure in 008-4", result == 0);

    // test_entry2 is now in list at position 1
    result = where_in_list(g_entries, test_entry2);
    mu_assert("Failure in 008-5", result == 1);

    free_entry_list(g_entries);

    return 0;
}


char *browser_init_009() {
    int cat_id = 0;
    
    // NULL el passed
    g_entries = NULL;
    g_browser = init_browser(g_entries, 10);
    mu_assert("Failure in 009-1", !g_browser->start);
    mu_assert("Failure in 009-2", !g_browser->sel);
    mu_assert("Failure in 009-3", !g_browser->end);
    free_browser(g_browser);

    // empty el passed
    g_entries = init_entry_list();
    g_browser = init_browser(g_entries, 10);
    mu_assert("Failure in 009-4", !g_browser->start);
    mu_assert("Failure in 009-5", !g_browser->sel);
    mu_assert("Failure in 009-6", !g_browser->end);
    free_browser(g_browser);

    // list with one item but not enough rows to fit it
    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry(1, "Starbucks", test_date1, -20.99, cat_id,
	    "Zionist scum!");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(g_entries, test_entry_node1);

    g_browser = init_browser(g_entries, 0);
    mu_assert("Failure in 009-7", !g_browser->start);
    mu_assert("Failure in 009-8", !g_browser->sel);
    mu_assert("Failure in 009-9", !g_browser->end);
    free_browser(g_browser);

    // list with one item and enough space for it
    g_browser = init_browser(g_entries, 1);
    mu_assert("Failure in 009-10", g_browser->start = g_entries->tail);
    mu_assert("Failure in 009-11", g_browser->sel = g_entries->tail);
    mu_assert("Failure in 009-12", g_browser->end = g_entries->tail);
    free_browser(g_browser);

    // two items but only room for 1
    struct tm test_time_tm2 = {.tm_mday=2, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date2 = mktime(&test_time_tm2);
    entry_t* test_entry2 = init_entry(2, "Metro", test_date2, -20.99, cat_id,
            NULL);
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(g_entries, test_entry_node2);
    g_browser = init_browser(g_entries, 1);
    mu_assert("Failure in 009-13", g_browser->start = g_entries->tail);
    mu_assert("Failure in 009-14", g_browser->sel = g_entries->tail);
    mu_assert("Failure in 009-15", g_browser->end = g_entries->tail);
    free_browser(g_browser);
    
    // two items with room for both 
    g_browser = init_browser(g_entries, 2);
    mu_assert("Failure in 009-16", g_browser->start = g_entries->head);
    mu_assert("Failure in 009-17", g_browser->sel = g_entries->tail);
    mu_assert("Failure in 009-18", g_browser->end = g_entries->tail);
    free_browser(g_browser);

    free_entry_list(g_entries);

    return 0;
}




char *is_end_node_010() {
    int cat_id = 1;

    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    mu_assert("Failure in 010-1", is_end_node(en1));

    entry_t* e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);

    // Both are still end nodes
    mu_assert("Failure in 010-2", is_end_node(en2));
    mu_assert("Failure in 010-3", is_end_node(en1));

    entry_t* e3 = init_entry(3, "Starbucks", d1, -0.31, cat_id,
	"Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    // 1 and 3 are end nodes but 2 isn't
    mu_assert("Failure in 010-4", is_end_node(en1));
    mu_assert("Failure in 010-5", is_end_node(en3));
    mu_assert("Failure in 010-6", !is_end_node(en2));
    mu_assert("Failure in 010-7", is_head(en1));
    mu_assert("Failure in 010-8", is_tail(en3));

    free_entry_list(g_entries);
    return 0;
}


char *entry_node_traverse_011() {
    int cat_id = 1;
    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    entry_t* e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);

    entry_t* e3 = init_entry(3, "Starbucks", d1, -0.31, cat_id,
	"Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    entry_node_t* temp = g_entries->tail;

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

    free_entry_list(g_entries);
    return 0;
}


char *browser_scroll_012() {
    int cat_id = 1;
    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    entry_t* e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);

    entry_t* e3 = init_entry(3, "Starbucks", d1, -0.31, cat_id,
	"Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    // g_browser with room for full list
    g_browser = init_browser(g_entries, 6);
    
    // can't scroll down
    browser_scroll(1, DOWN); 
    mu_assert("Failure in 012-1", g_browser->start == en1);
    mu_assert("Failure in 012-2", g_browser->sel == en3);
    mu_assert("Failure in 012-3", g_browser->end == en3);

    browser_scroll(1, UP); 
    mu_assert("Failure in 012-4", g_browser->start == en1);
    mu_assert("Failure in 012-5", g_browser->sel == en2);
    mu_assert("Failure in 012-6", g_browser->end == en3);

    // scroll beyond head
    browser_scroll(2, UP); 
    mu_assert("Failure in 012-7", g_browser->start == en1);
    mu_assert("Failure in 012-8", g_browser->sel == en1);
    mu_assert("Failure in 012-9", g_browser->end == en3);

    browser_scroll(2, DOWN); 
    mu_assert("Failure in 012-10", g_browser->start == en1);
    mu_assert("Failure in 012-11", g_browser->sel == en3);
    mu_assert("Failure in 012-12", g_browser->end == en3);

    free_browser(g_browser);

    // only room for bottom 2 in g_entries
    g_browser = init_browser(g_entries, 2);

    browser_scroll(1, UP); 
    mu_assert("Failure in 012-13", g_browser->start == en2);
    mu_assert("Failure in 012-14", g_browser->sel == en2);
    mu_assert("Failure in 012-15", g_browser->end == en3);

    // one more changes start end context
    browser_scroll(1, UP); 
    mu_assert("Failure in 012-16", g_browser->start == en1);
    mu_assert("Failure in 012-17", g_browser->sel == en1);
    mu_assert("Failure in 012-18", g_browser->end == en2);

    browser_scroll(2, DOWN); 
    mu_assert("Failure in 012-19", g_browser->start == en2);
    mu_assert("Failure in 012-20", g_browser->sel == en3);
    mu_assert("Failure in 012-21", g_browser->end == en3);

    free_entry_list(g_entries);
    free_browser(g_browser);

    return 0;
}


char *cat_array_013() {

    int rc;
    // empty category array
    g_categories = init_cat_array();
    mu_assert("Failure in 013-01", !g_categories->array);
    mu_assert("Failure in 013-02", !g_categories->num_cats);

    // Signature of (parent_id, id, name). 0 is NULL category
    category_t *c1 = init_category(1, 0, "Food");
    mu_assert("Failure in 013-03", c1->id == 1);
    mu_assert("Failure in 013-04", c1->parent_id == 0);
    mu_assert("Failure in 013-05", strcmp(c1->name, "Food") == 0);

    free_category(c1);

    // Parent category is NULL, ie Food will be a main category. legal
    c1 = init_category(1, 0, "Food");
    rc = append_to_cat_array(g_categories, c1);
    mu_assert("Failure in 013-06", g_categories->array[0] == c1);
    mu_assert("Failure in 013-07", !rc);

    // Parent category is Food, which is already in array. legal
    category_t *c2 = init_category(2, 1, "Cafe");
    append_to_cat_array(g_categories, c2);
    mu_assert("Failure in 013-08", g_categories->array[0] == c1);
    mu_assert("Failure in 013-09", g_categories->array[1] == c2);
    mu_assert("Failure in 013-10", !rc);

    // Parent category does not exist. should fail
    category_t *c3 = init_category(3, 3, "Cafe");
    rc = append_to_cat_array(g_categories, c3);
    mu_assert("Failure in 013-11", rc);
    mu_assert("Failure in 013-12", g_categories->num_cats == 2);
    free_category(c3);

    // Parent category is subcategory. should fail
    c3 = init_category(3, 2, "Cafe");
    rc = append_to_cat_array(g_categories, c3);
    mu_assert("Failure in 013-11", rc);
    mu_assert("Failure in 013-12", g_categories->num_cats == 2);
    free_category(c3);

    free_cat_array(g_categories);

    return 0;
}

char *cat_id_to_names_014() {

    char *cat, *subcat;

    g_categories = init_cat_array();
    category_t *c1 = init_category(1, 0, "Food");
    append_to_cat_array(g_categories, c1);

    cat_id_to_names(g_categories, 1, &cat, &subcat);
    mu_assert("Failure in 014-01", strcmp(cat, "Food") == 0);
    mu_assert("Failure in 014-02", !subcat);

    category_t *c2 = init_category(2, 1, "Cafe");
    append_to_cat_array(g_categories, c2);

    // category one is still just food without subcat
    cat_id_to_names(g_categories, 1, &cat, &subcat);
    mu_assert("Failure in 014-03", strcmp(cat, "Food") == 0);
    mu_assert("Failure in 014-04", !subcat);
    
    // category one is still just food without subcat
    cat_id_to_names(g_categories, 2, &cat, &subcat);
    mu_assert("Failure in 014-05", strcmp(cat, "Food") == 0);
    mu_assert("Failure in 014-06", strcmp(subcat, "Cafe") == 0);

    free_cat_array(g_categories);
    return 0;
}


char *date_proc_015() {

    char *test_string = "11/17/2023";
    time_t result;
    mu_assert("Failure in 015-01", !date_proc(test_string, &result));
    
    struct tm *result_tm = localtime(&result);
    mu_assert("Failure in 015-02", result_tm->tm_mday == 17);
    mu_assert("Failure in 015-03", result_tm->tm_mon == 11-1);
    mu_assert("Failure in 015-04", result_tm->tm_year == 2023 - 1900);

    // missing slashes
    test_string = "11172024";
    mu_assert("Failure in 015-05", date_proc(test_string, &result));

    // valid shortform
    test_string = "11/12";
    mu_assert("Failure in 015-06", !date_proc(test_string, &result));
    result_tm = localtime(&result);
    mu_assert("Failure in 015-07", result_tm->tm_mday == 12);
    mu_assert("Failure in 015-08", result_tm->tm_mon == 11-1);
    mu_assert("Failure in 015-09", result_tm->tm_year == 2024 - 1900);

    // invalid month
    test_string = "13/14";
    mu_assert("Failure in 015-10", date_proc(test_string, &result));

    // invalid day
    test_string = "11/31";
    mu_assert("Failure in 015-11", date_proc(test_string, &result));

    return 0;
}

char *amount_proc_016() {
    int rc;
    float result;
    char *test_str = "";

    // empty string 
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-01", rc == BUDGURSE_FAILURE);

    // cannot be 0
    test_str = "0.00";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-02", rc == BUDGURSE_FAILURE);

    test_str = "not a digit";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-03", rc == BUDGURSE_FAILURE);

    // legal float + non number
    test_str = "14.00 and not a digit";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-04", rc == BUDGURSE_FAILURE);
    test_str = "not a digit and 14.00";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-05", rc == BUDGURSE_FAILURE);

    // commas are not allowed
    test_str = "14,000.00";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-06", rc == BUDGURSE_FAILURE);
    
    // legal
    test_str = "14000.02";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-07", rc == BUDGURSE_SUCCESS);
    mu_assert("Failure in 016-08", amount_eq(result, 14000.02));

    // legal -ve
    test_str = "-12";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-09", rc == BUDGURSE_SUCCESS);
    mu_assert("Failure in 016-10", amount_eq(result, -12.00));

    // too large and small
    test_str = "1000000";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-11", rc == BUDGURSE_FAILURE);
    test_str = "-100000";
    rc = amount_proc(test_str, &result);
    mu_assert("Failure in 016-12", rc == BUDGURSE_FAILURE);

    return 0;
}


char *cat_proc_017() {

    g_categories = init_cat_array();
    
    int rc;
    int id;
    char *test_str = "";

    // can't be empty
    rc = m_cat_proc(test_str, &id);
    mu_assert("Failure in 017-01", rc == BUDGURSE_FAILURE);

    test_str = "Food";
    category_t *c1 = init_category(1, 0, test_str);
    append_to_cat_array(g_categories, c1);
    rc = m_cat_proc(test_str, &id);
    mu_assert("Failure in 017-02", rc == BUDGURSE_SUCCESS);

    test_str = "Max characterssssssssssssssssss";
    category_t *c2 = init_category(2, 0, test_str);
    append_to_cat_array(g_categories, c2);
    rc = m_cat_proc(test_str, &id);
    mu_assert("Failure in 017-03", rc == BUDGURSE_SUCCESS);
    mu_assert("Failure in 017-04", id == 2);

    test_str = "Too many characterssssssssssssss";
    category_t *c3 = init_category(3, 0, test_str);
    append_to_cat_array(g_categories, c3);
    rc = m_cat_proc(test_str, &id);
    mu_assert("Failure in 017-05", rc == BUDGURSE_FAILURE);

    // Can't be a subcategory
    test_str = "Cafe";
    category_t *c4 = init_category(4, 1, test_str);
    append_to_cat_array(g_categories, c4);
    rc = m_cat_proc(test_str, &id);
    mu_assert("Failure in 017-06", rc == BUDGURSE_FAILURE);

    // SUBCAT TESTS
    // Can't be main category
    test_str = "Food";
    rc = s_cat_proc(test_str, &id);
    mu_assert("Failure in 017-07", rc == BUDGURSE_FAILURE);

    // legal
    test_str = "Max characterssssssssssssssssss";
    // make it a subcat of Food
    category_t *c5 = init_category(5, 1, test_str);
    rc = append_to_cat_array(g_categories, c5);
    // id should be set to parent_id before entering fn
    id = 1;
    rc = s_cat_proc(test_str, &id);
    mu_assert("Failure in 017-08", rc == BUDGURSE_SUCCESS);
    mu_assert("Failure in 017-09", id == 5);

    // legal except too long
    test_str = "Too many characterssssssssssssss";
    category_t *c6 = init_category(6, 0, test_str);
    append_to_cat_array(g_categories, c6);
    rc = s_cat_proc(test_str, &id);
    mu_assert("Failure in 017-10", rc == BUDGURSE_FAILURE);

    // Subcat CAN be empty. id should be kept as is
    test_str = "";
    rc = s_cat_proc(test_str, &id);
    id = 1;
    mu_assert("Failure in 017-11", rc == BUDGURSE_SUCCESS);
    mu_assert("Failure in 017-12", id == 1);

    free_cat_array(g_categories);
    return 0;
}


char* cat_to_sql_insert_018() {
    
    category_t *c = init_category(1, 0, "Food");

    char* sql = cat_to_sql_insert(c);
    char* expected_statement = "INSERT INTO Categories (id, parent_id, name) "
        		       "VALUES (1, 0, 'Food');";

    mu_assert("Failure in 018-01", strcmp(sql, expected_statement) == 0);
    free_category(c);
    free(sql);

    return 0;
}

char *del_head_019() {
    int cat_id = 1;

    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    entry_t* e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);
    
    entry_t* e3 = init_entry(3, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    del_head(g_entries);
    mu_assert("Failure in 019-01", en2 == g_entries->head);
    mu_assert("Failure in 019-02", en2->prev == NULL);
    mu_assert("Failure in 019-03", g_entries->num_nodes == 2);

    del_head(g_entries);
    mu_assert("Failure in 019-04", g_entries->head == en3);
    mu_assert("Failure in 019-05", g_entries->tail == en3);
    mu_assert("Failure in 019-06", en3->prev == NULL);
    mu_assert("Failure in 019-07", g_entries->num_nodes == 1);

    del_head(g_entries);
    mu_assert("Failure in 019-08", g_entries->head == NULL);
    mu_assert("Failure in 019-09", g_entries->tail == NULL);
    mu_assert("Failure in 019-10", g_entries->num_nodes == 0);

    free_entry_list(g_entries);
    return 0;
}


char *del_entry_020() {
    int cat_id = 1;

    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t *en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    entry_t *e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t *en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);

    entry_t *e3 = init_entry(3, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t *en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    del_entry(g_entries, en2);
    mu_assert("Failure in 020-01", en1->next == en3);
    mu_assert("Failure in 020-02", en3->prev == en1);
    mu_assert("Failure in 020-03", g_entries->num_nodes == 2);

    del_entry(g_entries, en3);
    mu_assert("Failure in 020-04", g_entries->head == en1);
    mu_assert("Failure in 020-05", g_entries->tail == en1);
    mu_assert("Failure in 020-04", en1->next == NULL);
    mu_assert("Failure in 020-07", en1->prev == NULL);
    mu_assert("Failure in 020-08", g_entries->num_nodes == 1);

    del_entry(g_entries, en1);
    mu_assert("Failure in 020-09", g_entries->head == NULL);
    mu_assert("Failure in 020-010", g_entries->tail == NULL);
    mu_assert("Failure in 020-011", g_entries->num_nodes == 0);

    free_entry_list(g_entries);
    return 0;
}

char *del_entry_to_sql_021() {
    int cat_id = 1;
    int id = 1;
    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t *e = init_entry(id, "Starbucks", d1, -0.21, cat_id, NULL);

    char *sql = del_entry_to_sql(e);
    char *expected_statement = "DELETE FROM Entries WHERE id=1;";

    mu_assert("Failure in 021-01", strcmp(sql, expected_statement) == 0);
    free_entry(e);
    free(sql);

    return 0;
}


char *browser_del_entry_022() {

    int cat_id = 1;

    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t *en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    entry_t *e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t *en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);

    entry_t *e3 = init_entry(3, "Starbucks", d1, -0.21, cat_id, NULL);
    entry_node_t *en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------

    g_browser = init_browser(g_entries, 12);

    // delete middle
    // move selected to middle
    entry_node_traverse(&g_browser->sel, UP);
    // delete selected 
    browser_del_entry();

    mu_assert("Failure in 022-01", g_browser->sel == en3);
    mu_assert("Failure in 022-02", g_browser->end == en3);
    mu_assert("Failure in 022-03", g_browser->start == en1);
    mu_assert("Failure in 022-04", g_browser->num_entries == 2);

    // reset browser
    entry_node_traverse(&g_browser->sel, DOWN);
    entry_node_traverse(&g_browser->end, DOWN);
    g_browser->num_entries++;
    // el tail
    browser_del_entry();

    mu_assert("Failure in 022-05", g_browser->sel == en2);
    mu_assert("Failure in 022-06", g_browser->end == en2);
    mu_assert("Failure in 022-07", g_browser->start == en1);
    mu_assert("Failure in 022-08", g_browser->num_entries == 2);

    // reset browser
    entry_node_traverse(&g_browser->end, DOWN);
    entry_node_traverse(&g_browser->sel, DOWN);
    (g_browser->num_entries)++;
    // move to head and delete head
    entry_node_traverse(&g_browser->sel, UP);
    entry_node_traverse(&g_browser->sel, UP);
    browser_del_entry();

    mu_assert("Failure in 022-09", g_browser->sel == en2);
    mu_assert("Failure in 022-10", g_browser->end == en3);
    mu_assert("Failure in 022-11", g_browser->start == en2);
    mu_assert("Failure in 022-12", g_browser->num_entries == 2);

    free_browser(g_browser);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------
    
    // not enough space for one of three nodes
    g_browser = init_browser(g_entries, 2);

    // delete tail
    browser_del_entry();

    mu_assert("Failure in 022-13", g_browser->sel == en2);
    mu_assert("Failure in 022-14", g_browser->end == en2);
    mu_assert("Failure in 022-15", g_browser->start == en1);
    mu_assert("Failure in 022-16", g_browser->num_entries == 2);

    // reset browser
    entry_node_traverse(&g_browser->sel, DOWN);
    entry_node_traverse(&g_browser->start, DOWN);
    entry_node_traverse(&g_browser->end, DOWN);
    // move up and delete non-tail
    entry_node_traverse(&g_browser->sel, UP);
    browser_del_entry();

    mu_assert("Failure in 022-17", g_browser->sel == en1);
    mu_assert("Failure in 022-18", g_browser->end == en3);
    mu_assert("Failure in 022-19", g_browser->start == en1);
    mu_assert("Failure in 022-20", g_browser->num_entries == 2);

    free_entry_list(g_entries);
    free_browser(g_browser);
    
    // ------------------------------------------------------------------------
    // Delete from empty list
    // ------------------------------------------------------------------------

    g_entries = init_entry_list();
    g_browser = init_browser(g_entries, 2);

    mu_assert("Failure in 022-21", g_browser->sel == NULL);
    mu_assert("Failure in 022-22", g_browser->end == NULL);
    mu_assert("Failure in 022-23", g_browser->start == NULL);
    mu_assert("Failure in 022-24", g_browser->num_entries == 0);

    free_entry_list(g_entries);
    free_browser(g_browser);
    return 0;
}

/*
char *edit_entry_to_sql_update_023() {
    int cat_id = 1;
    int id = 1;
    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900, 
	.tm_isdst = 1}; 
    time_t d1 = mktime(&tm1);
    entry_t *e = init_entry(id, "Starbucks", d1, -0.21, cat_id, NULL);

    char *sql = edit_entry_to_sql_update(e);
    char* expected_statement = "UPDATE Entries SET name = 'Starbucks', "
			       "date = 1644642000, amount = -0.21, "
        		       "category_id = 1, note = '' WHERE"
        		       " id=1;"; 

    mu_assert("Failure in 023-01", strcmp(sql, expected_statement) == 0);
    free_entry(e);
    free(sql);
    return 0;
}
*/

void end_budgurse(int status) {
    
    free_browser(g_browser);
    free_cat_array(g_categories);
    free_entry_list(g_entries);
    free_wins(g_wins);
    if (sqlite3_close(g_db))
	ERROR_MSG("Failed to properly close database with error message: %s\n",
		  sqlite3_errmsg(g_db));

    // stop ncurses
    endwin();
    exit(status);
}
