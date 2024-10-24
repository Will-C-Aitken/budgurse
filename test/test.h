// modified from https://jera.com/techinfo/jtns/jtn002

#include "budgurse.h"

// Unit tests
#define mu_assert(test, test_suite, test_num) \
    do { tests_run++; \
	if (!(test)) { \
	    print_err_str(test_suite, __func__, test_num); \
	    return 1; \
	} \
   } while (0)

#define mu_run_test(test) do { int result = test(); \
                            if (result) return result; } while (0)

void print_err_str(const char *test_suite, const char *test_fn_name, 
	int test_num);

extern int tests_run;

// required data structure globals
extern entry_list_t *g_entries;
extern cat_array_t *g_categories;
extern sqlite3 *g_db;
extern browser_t *g_browser;
extern win_t wins[NUM_WINS];

extern int curses_mode;

// Entries Tests
int entries_tests();
int append_to_tail_test();
int del_tail_test();
int free_entry_list_test();
int where_in_list_test();
int is_end_node_test();
int entry_node_traverse_test();
int del_head_test();
int del_entry_test();

// Backend tests
int backend_tests();
int entry_to_sql_insert_test();
int load_empty_db_test();
int write_entry_test();
int load_db_test();
int cat_to_sql_insert_test();
int del_entry_to_sql_test();
// int edit_entry_to_sql_update_023();

// Browser tests
int browser_tests();
int browser_init_test();
int browser_scroll_test();
int browser_del_entry_test();

// Categories tests
int categories_tests();
int cat_array_test();
int cat_id_to_names_test();

// Prompt tests
int prompt_tests();
int date_proc_test();
int amount_proc_test();
int cat_proc_test();

// Utility functions
entry_list_t* test_dummy_entry_list(int num_nodes);
entry_t *test_dummy_entry(int id);
int is_amount_eq(float candidate, float actual);
