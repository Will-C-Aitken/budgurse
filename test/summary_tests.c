#include "test.h"


int summary_tests() {
    mu_run_test(summary_init_test);
    return 0;
}


int summary_init_test() {

    // Uncommenting these lines will cause program to exit
    // g_categories = NULL;
    // g_summary = init_summary(g_entries, WEEK, 10, 79);
    g_categories = test_dummy_cat_array(5);
/*
+------------------------------------79width---------------------------------+
|                    | Month 10    Month 11    Month 12    Total       Avg   |
+------------------------------------79width---------------------------------+
| CAT_STR_LEN......  | AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN AMNT_ |
|  SUCAT_STR_LEN.... | AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN AMNT_ |
+------------------------------------79width---------------------------------+
*/
    // Not enough room for all categories and just enough room for 4 col
    g_summary = init_summary(MONTH, 6, 73);
    mu_assert(g_summary->num_rows == 6, "Summary", 1);
    mu_assert(g_summary->num_cols == 14, "Summary", 2);
    mu_assert(g_summary->x_start == 9, "Summary", 3);
    mu_assert(g_summary->x_sel == 13, "Summary", 4);
    mu_assert(g_summary->x_end == 13, "Summary", 5);
    mu_assert(g_summary->y_start == 4, "Summary", 6);
    mu_assert(g_summary->y_sel == 5, "Summary", 7);
    mu_assert(g_summary->y_end == 5, "Summary", 8);
    mu_assert(is_amount_eq(g_summary->data[5*g_summary->num_rows + 13], 0.00), 
	    "Summary", 9);
    free_summary(g_summary);

    // room for all cats but not for just not enough for 4 cols
    g_summary = init_summary(MONTH, 18, 72);
    mu_assert(g_summary->num_rows == 6, "Summary", 10);
    mu_assert(g_summary->num_cols == 14, "Summary", 11);
    mu_assert(g_summary->x_start == 10, "Summary", 12);
    mu_assert(g_summary->x_sel == 13, "Summary", 13);
    mu_assert(g_summary->x_end == 13, "Summary", 14);
    mu_assert(g_summary->y_start == 0, "Summary", 15);
    mu_assert(g_summary->y_sel == 5, "Summary", 16);
    mu_assert(g_summary->y_end == 5, "Summary", 17);
    mu_assert(is_amount_eq(g_summary->data[0], 0.00), "Summary", 9);
    free_summary(g_summary);
    free_cat_array(g_categories);
    return 0;
}
