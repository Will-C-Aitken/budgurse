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
	    "Coffee Shop", "this was fun");
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
	    "Coffee Shop", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Coffee Shop", "this was fun");
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
	    "Coffee Shop", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = init_entry("Tims", test_date, -12.00, "Food", 
	    "Coffee Shop", "this was fun");
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
	    "Coffee Shop", "this was fun");

    char* sql = entry_to_sql_insert(test_entry1);
    char* expected_statement = "INSERT INTO ENTRIES VALUES ('Starbucks', "
			       "1644642000, -12.00, 'Food', 'Coffee Shop', " 
			       "'this was fun');"; 

    mu_assert("Failure in 004-01", strcmp(sql, expected_statement) == 0);
    free_entry(test_entry1);
    free(sql);

    return 0;
}


char* write_entry_005() {
    
    // sec, min, hour, day, month, year, is_dst
    struct tm test_time_tm1 = {0, 0, 0, 12, 1, 2022 - 1900, 1};
    time_t test_date = mktime(&test_time_tm1);
    entry_t* test_entry1 = init_entry("Starbucks", test_date, -12.00, "Food", 
	    "Coffee Shop", "this was fun");

    sqlite3 *db = NULL;
    int result = init_db(&db, "test/test.db");
    mu_assert("Failure in 005-01", result == SQLITE_OK);

    result = write_entry(db, test_entry1);
    mu_assert("Failure in 005-02", result);
    
    result = sqlite3_close(db);
    mu_assert("Failure in 005-03", result == SQLITE_OK);
    free_entry(test_entry1);

    return 0;
}


static char* all_tests() {
    mu_run_test(test_append_to_tail_001);
    mu_run_test(free_head_002);
    mu_run_test(free_list_003);
    mu_run_test(entry_to_sql_insert_004);
    mu_run_test(write_entry_005);
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

