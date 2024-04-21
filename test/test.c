#include "test.h"

int tests_run = 0;


char* test_append_to_tail_001() {
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

    entry_t* test_entry2 = get_tail(test_list);
    mu_assert("Failure in 001", strcmp(test_entry2->name, "Starbucks") == 0);
    free_head(test_list);
    free(test_list);

    return 0;
}


char* free_head_002() {
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

    free_head(test_list);
    mu_assert("Failure in 002-01", test_list->num_nodes == 1);

    free_head(test_list);
    mu_assert("Failure in 002-02", test_list->num_nodes == 0);
    free_head(test_list);
    free(test_list);

    return 0;

}


char* free_list_003() {
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

    free_list(test_list);

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

    free_list(entries);
    result = sqlite3_close(db);
    mu_assert("Failure in 006-04", result == SQLITE_OK);

    return 0;
}


// First write normal entry, then entry with NULL members
char* write_entry_006() {

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

    // check head 
    entry_t* head = entries->head->data;
    mu_assert("Failure in 007-07", strcmp(head->name, "Starbucks") == 0);
    mu_assert("Failure in 007-08", head->amount == -12.00);

    struct tm test_time_tm2 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_time2 = mktime(&test_time_tm2);
    mu_assert("Failure in 007-09", difftime(test_time2, head->date) == 0.0);

    free_list(entries);

    result = sqlite3_close(db);
    mu_assert("Failure in 007-10", result == SQLITE_OK);

    return 0;
}


static char* all_tests() {
    mu_run_test(test_append_to_tail_001);
    mu_run_test(free_head_002);
    mu_run_test(free_list_003);
    mu_run_test(entry_to_sql_insert_004);
    mu_run_test(load_empty_db_005);
    mu_run_test(write_entry_006);
    mu_run_test(load_db_007);
    return 0;
}


int main(int argc, char **argv) {
     char *result = all_tests();

     if (result != 0)
         printf("%s\n", result);
     else
         printf("ALL TESTS PASSED\n");

     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
}

