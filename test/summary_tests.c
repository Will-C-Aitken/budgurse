#include "test.h"


int summary_tests() {
    mu_run_test(summary_init_test);
    mu_run_test(summary_calc_test);
    mu_run_test(summary_scroll_test);
    mu_run_test(update_date_test);
    mu_run_test(summary_clear_test);
    return 0;
}


int summary_init_test() {

    // Uncommenting these lines will cause program to exit
    // g_categories = NULL;
    // g_summary = init_summary(0, WEEK, 10, 79);
    // g_entries = NULL;
    // g_summary = init_summary(0, WEEK, 10, 79);
    g_categories = test_dummy_cat_list(5);
/*
+------------------------------------79width---------------------------------+
|                    | Month 10    Month 11    Month 12      | Total         |
+------------------------------------79width---------------------------------+
| CAT_STR_LEN......  | AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN   | AMNT_STR_LN   |
|  SUCAT_STR_LEN.... | AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN   | AMNT_STR_LN   |
+------------------------------------79width---------------------------------+
|  TOTAL             | AMNT_STR_LN AMNT_STR_LN AMNT_STR_LN   | AMNT_STR_LN   |
+------------------------------------79width---------------------------------+
*/
    // Not enough room for all categories and just enough room for 4 col
    time_t cur_time = time(NULL);
    struct tm *cur_tm = localtime(&cur_time);
    clean_tm(cur_tm);
    cur_tm->tm_mday = days_in_mnth[cur_tm->tm_mon];
    time_t max_date = mktime(cur_tm);
    cur_tm->tm_mday = 1;
    cur_tm->tm_mon += 1;
    if (cur_tm->tm_mon < 12)
	cur_tm->tm_year--;
    cur_tm->tm_mon %= 12;
    time_t min_date = mktime(cur_tm);
    g_summary = init_summary(cur_time, MONTH, 8, 77);
    mu_assert(g_summary->max_date == max_date, "Summary", 1);
    mu_assert(g_summary->min_date == min_date, "Summary", 2);
    mu_assert(g_summary->delin == MONTH, "Summary", 3);
    mu_assert(g_summary->num_rows == 6, "Summary", 4);
    mu_assert(g_summary->num_cols == 13, "Summary", 5);
    mu_assert(g_summary->x_start == 9, "Summary", 6);
    mu_assert(g_summary->x_sel == 12, "Summary", 7);
    mu_assert(g_summary->x_end == 12, "Summary", 8);
    mu_assert(g_summary->y_start == 3, "Summary", 9);
    mu_assert(g_summary->y_sel == 5, "Summary", 10);
    mu_assert(g_summary->y_end == 5, "Summary", 11);
    // last pos
    mu_assert(is_amount_eq(g_summary->data[5*g_summary->num_rows + 12], 0.00), 
	    "Summary", 12);
    free_summary(g_summary);

    // room for all cats but just not enough for 4 cols
    g_summary = init_summary(0, MONTH, 18, 76);
    mu_assert(g_summary->num_rows == 6, "Summary", 13);
    mu_assert(g_summary->num_cols == 13, "Summary", 14);
    mu_assert(g_summary->x_start == 10, "Summary", 15);
    mu_assert(g_summary->x_sel == 12, "Summary", 16);
    mu_assert(g_summary->x_end == 12, "Summary", 17);
    mu_assert(g_summary->y_start == 0, "Summary", 18);
    mu_assert(g_summary->y_sel == 5, "Summary", 19);
    mu_assert(g_summary->y_end == 5, "Summary", 20);
    // first pos
    mu_assert(is_amount_eq(g_summary->data[0], 0.00), "Summary", 21);

    free_summary(g_summary);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);

    // empty cat list 
    g_categories = test_dummy_cat_list(0);
    g_summary = init_summary(0, MONTH, 18, 76);
    mu_assert(g_summary->num_rows == 1, "Summary", 22);
    mu_assert(g_summary->num_cols == 13, "Summary", 23);
    mu_assert(g_summary->x_start == 10, "Summary", 24);
    mu_assert(g_summary->x_sel == 12, "Summary", 25);
    mu_assert(g_summary->x_end == 12, "Summary", 26);
    mu_assert(g_summary->y_start == 0, "Summary", 27);
    mu_assert(g_summary->y_sel == 0, "Summary", 28);
    mu_assert(g_summary->y_end == 0, "Summary", 29);
    free_summary(g_summary);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);

    return 0;
}


int summary_calc_test() {
    g_categories = test_dummy_cat_list(3);
    g_entries = init_llist();

    // populate entry list that will result in this table:
/*
+------------------------------------79width---------------------------------+
|                    |     Month 1     Month 2       ...         Total       |
+------------------------------------79width---------------------------------+
| Cat1               |      -12.00	-13.00       ... 	-25.00       |
|  Cat2              |	      0.00       -5.00			 -5.00       |
| Cat3               |	    450.00     -100.00			350.00       |
| Total              |      438.00     -113.00			325.00       |
+------------------------------------79width---------------------------------+
*/
    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=12, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -10.00, 
	    cat_get_from_id(g_categories, 1), "A Note");
    llist_node_t *en1 = init_llist_node(e1);
    llist_insert_node(g_entries, en1, (llist_comp_fn_t)entry_date_comp_gte);
    entry_t *e2 = init_entry(2, "A Name", date1, -2.00, 
	    cat_get_from_id(g_categories, 1), "A Note");
    llist_node_t *en2 = init_llist_node(e2);
    llist_insert_node(g_entries, en2, (llist_comp_fn_t)entry_date_comp_gte);
    entry_t *e3 = init_entry(3, "A Name", date1, 450.00, 
	    cat_get_from_id(g_categories, 3), "A Note");
    llist_node_t *en3 = init_llist_node(e3);
    llist_insert_node(g_entries, en3, (llist_comp_fn_t)entry_date_comp_gte);

    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e4 = init_entry(4, "A Name", date2, -8.00, 
	    cat_get_from_id(g_categories, 1), "A Note");
    llist_node_t *en4 = init_llist_node(e4);
    llist_insert_node(g_entries, en4, (llist_comp_fn_t)entry_date_comp_gte);
    entry_t *e5 = init_entry(5, "A Name", date2, -5.00, 
	    cat_get_from_id(g_categories, 2), "A Note");
    llist_node_t *en5 = init_llist_node(e5);
    llist_insert_node(g_entries, en5, (llist_comp_fn_t)entry_date_comp_gte);
    entry_t *e6 = init_entry(6, "A Name", date2, -100.00, 
	    cat_get_from_id(g_categories, 3), "A Note");
    llist_node_t *en6 = init_llist_node(e6);
    llist_insert_node(g_entries, en6, (llist_comp_fn_t)entry_date_comp_gte);

    g_summary = init_summary(date2, MONTH, 18, 72);
    summary_calc();
    
    mu_assert(is_amount_eq(g_summary->data[0], -12.00), "Summary", 31);
    mu_assert(is_amount_eq(g_summary->data[1], -13.00), "Summary", 32);
    mu_assert(is_amount_eq(g_summary->data[12], -25.00), "Summary", 33);
    mu_assert(is_amount_eq(g_summary->data[13], 0.00), "Summary", 34);
    mu_assert(is_amount_eq(g_summary->data[14], -5.00), "Summary", 35);
    mu_assert(is_amount_eq(g_summary->data[25], -5.00), "Summary", 36);
    mu_assert(is_amount_eq(g_summary->data[26], 450.00), "Summary", 37);
    mu_assert(is_amount_eq(g_summary->data[27], -100.00), "Summary", 38);
    mu_assert(is_amount_eq(g_summary->data[38], 350.00), "Summary", 39);
    mu_assert(is_amount_eq(g_summary->data[39], 438.00), "Summary", 40);
    mu_assert(is_amount_eq(g_summary->data[40], -113.00), "Summary", 41);
    mu_assert(is_amount_eq(g_summary->data[51], 325.00), "Summary", 42);

    free_summary(g_summary);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    free_llist(g_entries, (llist_free_data_fn_t)free_entry);

    return 0;
}


int summary_scroll_test() {
    g_categories = test_dummy_cat_list(3);

    // ------------------------------------------------------------------------
    // First check bounds of non-sliding version
    // ------------------------------------------------------------------------
   
    // has room for all rows and all cols
    g_summary = init_summary(0, MONTH, 10, 256);

    // already at bottom and right, should do nothing
    summary_scroll(1, DOWN);
    summary_scroll(1, RIGHT);
    mu_assert(g_summary->x_start == 0, "Summary", 43);
    mu_assert(g_summary->x_sel == 12, "Summary", 44);
    mu_assert(g_summary->x_end == 12, "Summary", 45);
    mu_assert(g_summary->y_start == 0, "Summary", 46);
    mu_assert(g_summary->y_sel == 3, "Summary", 47);
    mu_assert(g_summary->y_end == 3, "Summary", 48);

    // left 11 times should bring to col idx 1
    summary_scroll(11, LEFT);
    mu_assert(g_summary->x_start == 0, "Summary", 49);
    mu_assert(g_summary->x_sel == 1, "Summary", 50);
    mu_assert(g_summary->x_end == 12, "Summary", 51);
    mu_assert(g_summary->y_start == 0, "Summary", 52);
    mu_assert(g_summary->y_sel == 3, "Summary", 53);
    mu_assert(g_summary->y_end == 3, "Summary", 54);

    // two more times would be beyond left edge. should stop at 0
    summary_scroll(2, LEFT);
    mu_assert(g_summary->x_start == 0, "Summary", 55);
    mu_assert(g_summary->x_sel == 0, "Summary", 56);
    mu_assert(g_summary->x_end == 12, "Summary", 57);
    mu_assert(g_summary->y_start == 0, "Summary", 58);
    mu_assert(g_summary->y_sel == 3, "Summary", 59);
    mu_assert(g_summary->y_end == 3, "Summary", 60);

    // same deal up
    summary_scroll(2, UP);
    mu_assert(g_summary->x_start == 0, "Summary", 61);
    mu_assert(g_summary->x_sel == 0, "Summary", 62);
    mu_assert(g_summary->x_end == 12, "Summary", 63);
    mu_assert(g_summary->y_start == 0, "Summary", 64);
    mu_assert(g_summary->y_sel == 1, "Summary", 65);
    mu_assert(g_summary->y_end == 3, "Summary", 66);
    summary_scroll(2, UP);
    mu_assert(g_summary->x_start == 0, "Summary", 67);
    mu_assert(g_summary->x_sel == 0, "Summary", 68);
    mu_assert(g_summary->x_end == 12, "Summary", 69);
    mu_assert(g_summary->y_start == 0, "Summary", 70);
    mu_assert(g_summary->y_sel == 0, "Summary", 71);
    mu_assert(g_summary->y_end == 3, "Summary", 72);

    free_summary(g_summary);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);

    // ------------------------------------------------------------------------
    // Now check scrolling context bounds as well
    // ------------------------------------------------------------------------

    g_categories = test_dummy_cat_list(10);
    // only room for 3 visible rows and 4 cols
    g_summary = init_summary(0, MONTH, 8, 77);

    // past the top
    summary_scroll(11, UP);
    mu_assert(g_summary->x_start == 9, "Summary", 73);
    mu_assert(g_summary->x_sel == 12, "Summary", 74);
    mu_assert(g_summary->x_end == 12, "Summary", 75);
    mu_assert(g_summary->y_start == 0, "Summary", 76);
    mu_assert(g_summary->y_sel == 0, "Summary", 77);
    mu_assert(g_summary->y_end == 2, "Summary", 78);

    // down, but not enough to shift context to bottom. the selected row should
    // not move beyond y_end - 1, in effect freezing the bottom row 
    summary_scroll(7, DOWN);
    mu_assert(g_summary->x_start == 9, "Summary", 79);
    mu_assert(g_summary->x_sel == 12, "Summary", 80);
    mu_assert(g_summary->x_end == 12, "Summary", 81);
    mu_assert(g_summary->y_start == 6, "Summary", 82);
    mu_assert(g_summary->y_sel == 7, "Summary", 83);
    mu_assert(g_summary->y_end == 8, "Summary", 84);

    // move left, but not to end 
    summary_scroll(7, LEFT);
    mu_assert(g_summary->x_start == 5, "Summary", 85);
    mu_assert(g_summary->x_sel == 5, "Summary", 86);
    mu_assert(g_summary->x_end == 8, "Summary", 87);
    mu_assert(g_summary->y_start == 6, "Summary", 88);
    mu_assert(g_summary->y_sel == 7, "Summary", 89);
    mu_assert(g_summary->y_end == 8, "Summary", 90);

    // move left beyond end 
    summary_scroll(7, LEFT);
    mu_assert(g_summary->x_start == 0, "Summary", 91);
    mu_assert(g_summary->x_sel == 0, "Summary", 92);
    mu_assert(g_summary->x_end == 3, "Summary", 93);
    mu_assert(g_summary->y_start == 6, "Summary", 94);
    mu_assert(g_summary->y_sel == 7, "Summary", 95);
    mu_assert(g_summary->y_end == 8, "Summary", 96);

    // as with moving down, last col should stay ahead (frozen) when moving 
    // right
    summary_scroll(5, RIGHT);
    mu_assert(g_summary->x_start == 3, "Summary", 97);
    mu_assert(g_summary->x_sel == 5, "Summary", 98);
    mu_assert(g_summary->x_end == 6, "Summary", 99);
    mu_assert(g_summary->y_start == 6, "Summary", 100);
    mu_assert(g_summary->y_sel == 7, "Summary", 101);
    mu_assert(g_summary->y_end == 8, "Summary", 102);

    free_summary(g_summary);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);

    return 0;
}


int update_date_test() {
    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=12, .tm_mon=11, .tm_year=2022 - 1900, .tm_isdst=0}; 
    time_t date1 = mktime(&tm1);

    update_date(&date1, MONTH, 1);
    struct tm val_tm = *localtime(&date1);
    mu_assert(val_tm.tm_sec == 0, "Summary", 103);
    mu_assert(val_tm.tm_min == 0, "Summary", 104);
    mu_assert(val_tm.tm_hour == 0, "Summary", 105);
    mu_assert(val_tm.tm_mday == 31, "Summary", 106);
    mu_assert(val_tm.tm_mon == 0, "Summary", 107);
    mu_assert(val_tm.tm_year == 2022 - 1900 + 1, "Summary", 108);
    mu_assert(val_tm.tm_isdst == 0, "Summary", 109);

    update_date(&date1, YEAR, 1);
    val_tm = *localtime(&date1);
    mu_assert(val_tm.tm_sec == 0, "Summary", 110);
    mu_assert(val_tm.tm_min == 0, "Summary", 111);
    mu_assert(val_tm.tm_hour == 0, "Summary", 112);
    mu_assert(val_tm.tm_mday == 31, "Summary", 113);
    mu_assert(val_tm.tm_mon == 0, "Summary", 114);
    mu_assert(val_tm.tm_year == 2022 - 1900 + 2, "Summary", 115);
    mu_assert(val_tm.tm_isdst == 0, "Summary", 116);

    update_date(&date1, YEAR, -1);
    val_tm = *localtime(&date1);
    mu_assert(val_tm.tm_sec == 0, "Summary", 117);
    mu_assert(val_tm.tm_min == 0, "Summary", 118);
    mu_assert(val_tm.tm_hour == 0, "Summary", 119);
    mu_assert(val_tm.tm_mday == 31, "Summary", 120);
    mu_assert(val_tm.tm_mon == 0, "Summary", 121);
    mu_assert(val_tm.tm_year == 2022 - 1900 + 1, "Summary", 122);
    mu_assert(val_tm.tm_isdst == 0, "Summary", 123);

    update_date(&date1, MONTH, -1);
    val_tm = *localtime(&date1);
    mu_assert(val_tm.tm_sec == 0, "Summary", 124);
    mu_assert(val_tm.tm_min == 0, "Summary", 125);
    mu_assert(val_tm.tm_hour == 0, "Summary", 126);
    mu_assert(val_tm.tm_mday == 31, "Summary", 127);
    mu_assert(val_tm.tm_mon == 11, "Summary", 128);
    mu_assert(val_tm.tm_year == 2022 - 1900, "Summary", 129);
    mu_assert(val_tm.tm_isdst == 0, "Summary", 130);

    return 0;
}


int summary_clear_test() {
    g_categories = test_dummy_cat_list(5);
    g_summary = init_summary(0, MONTH, 8, 77);
    // max location
    g_summary->data[5*g_summary->num_rows + 12] = 1.00; 
    summary_clear();
    mu_assert(is_amount_eq(g_summary->data[5*g_summary->num_rows + 12], 
		0.00), "Summary", 131);
    free_summary(g_summary);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    return 0;
}
