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
extern int curses_mode;

// Llist Tests
int llist_tests();
int append_to_tail_test();
int del_tail_test();
int free_entry_list_test();
int is_end_node_test();
int entry_node_traverse_test();
int del_head_test();
int del_entry_test();
int dist_between_test();

// Entries Tests
int entries_tests();
int insert_after_date_test();

// Backend tests
int backend_tests();
int entry_to_sql_insert_test();
int load_empty_db_test();
int write_entry_test();
int load_db_test();
int cat_to_sql_insert_test();
int del_entry_to_sql_test();
int edit_entry_to_sql_update_test();

// Browser tests
int browser_tests();
int browser_init_test();
int browser_scroll_test();
int browser_append_to_tail_test();
int browser_pop_sel_entry_test();
int browser_insert_after_date_test();

// Categories tests
int categories_tests();
int init_categories_test();
int cat_id_to_names_test();
int cat_name_to_id_test();
int cat_is_sub_test();
int cat_set_sum_idxs_test();
int cat_flatten_names_test();

// Prompt tests
int prompt_tests();
int date_proc_test();
int amount_proc_test();
int cat_proc_test();

// Summary tests
int summary_tests();
int summary_init_test();
int summary_calc_test();
int summary_scroll_test();

// Utility functions
llist_t* test_dummy_list(int num_nodes);
llist_t* test_dummy_cat_list(int num_cats);
int is_amount_eq(float candidate, float actual);
