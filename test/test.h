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

#ifndef TEST_H
#define TEST_H

#include "budgurse.h"

// modified from https://jera.com/techinfo/jtns/jtn002 
// (there is no official license)
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
int get_matches_test();
int sort_entry_test();

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
int cat_llist_to_array_test();
int cat_get_from_name_test();
int cat_del_from_llist_test();

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
int update_date_test();
int summary_clear_test();

// Test utility functions
llist_t* test_dummy_list(int num_nodes);
llist_t* test_dummy_cat_list(int num_cats);
int is_amount_eq(float candidate, float actual);

#endif
