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

#include "test.h"

int entries_tests() {
    mu_run_test(insert_after_date_test);
    mu_run_test(get_matches_test);
    mu_run_test(sort_entry_test);
    return 0;
}


int insert_after_date_test() {

    g_entries = init_llist();

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=12, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, NULL, "A Note");
    llist_node_t *en1= init_llist_node(e1);
    llist_insert_node(g_entries, en1, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 1, "Entries", 1);
    mu_assert(g_entries->head == en1, "Entries", 2);
    mu_assert(g_entries->tail == en1, "Entries", 3);

    // two days newer
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=14, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e2 = init_entry(2, "A Name", date2, -12.00, NULL, "A Note");
    llist_node_t *en2= init_llist_node(e2);
    llist_insert_node(g_entries, en2, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 2, "Entries", 4);
    mu_assert(g_entries->head == en1, "Entries", 5);
    mu_assert(g_entries->head->next == en2, "Entries",6);
    mu_assert(g_entries->tail == en2, "Entries", 7);
    mu_assert(g_entries->tail->prev == en1, "Entries", 8);

    // same time
    entry_t *e3 = init_entry(3, "A Name", date2, -12.00, NULL, "A Note");
    llist_node_t *en3= init_llist_node(e3);
    llist_insert_node(g_entries, en3, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 3, "Entries", 9);
    mu_assert(g_entries->head == en1, "Entries", 10);
    mu_assert(g_entries->head->next == en2, "Entries", 11);
    mu_assert(g_entries->tail == en3, "Entries", 12);
    mu_assert(g_entries->tail->prev == en2, "Entries", 13);
    mu_assert(en2->next == en3, "Entries", 14);
    mu_assert(en2->prev == en1, "Entries", 15);

    // one day before (should be second now)
    struct tm tm4 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=13, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date4 = mktime(&tm4);
    entry_t *e4 = init_entry(4, "A Name", date4, -12.00, NULL, "A Note");
    llist_node_t *en4= init_llist_node(e4);
    llist_insert_node(g_entries, en4, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 4, "Entries", 16);
    mu_assert(g_entries->head == en1, "Entries", 17);
    mu_assert(g_entries->head->next == en4, "Entries", 18);
    mu_assert(en4->next == en2, "Entries", 19);
    mu_assert(en4->prev == en1, "Entries", 20);
    mu_assert(en2->prev == en4, "Entries", 21);
    
    // one year before (should be first)
    struct tm tm5 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=13, .tm_mon=0, .tm_year=2021 - 1900, .tm_isdst=1}; 
    time_t date5 = mktime(&tm5);
    entry_t *e5 = init_entry(5, "A Name", date5, -12.00, NULL, "A Note");
    llist_node_t *en5= init_llist_node(e5);
    llist_insert_node(g_entries, en5, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 5, "Entries", 22);
    mu_assert(g_entries->head == en5, "Entries", 23);
    mu_assert(g_entries->head->next == en1, "Entries", 24);
    mu_assert(en1->prev == en5, "Entries", 25);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    return 0;
}


int get_matches_test() {
    category_t *c1 = init_category(1, 0, "Food");
    category_t *c2 = init_category(2, 0, "Home");
    category_t *c3 = init_category(3, 0, "Electronics");

    g_entries = init_llist();

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=12, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, c1, "A Note");
    llist_node_t *en1= init_llist_node(e1);
    llist_insert_to_tail(g_entries, en1);

    entry_t *e2 = init_entry(2, "A Name", date1, -12.00, c2, "A Note");
    llist_node_t *en2= init_llist_node(e2);
    llist_insert_to_tail(g_entries, en2);

    llist_t *m_ll = llist_get_matches(g_entries, c1, 
	(llist_cond_fn_t)entry_cat_cond_eq);
    mu_assert(m_ll->num_nodes == 1, "Entries", 26);
    mu_assert(m_ll->tail->data == g_entries->head->data, "Entries", 27);
    free_llist(m_ll, (llist_free_data_fn_t)free_nop);

    m_ll = llist_get_matches(g_entries, c2, 
	(llist_cond_fn_t)entry_cat_cond_eq);
    mu_assert(m_ll->num_nodes == 1, "Entries", 28);
    mu_assert(m_ll->head->data == g_entries->tail->data, "Entries", 29);
    free_llist(m_ll, (llist_free_data_fn_t)free_nop);

    m_ll = llist_get_matches(g_entries, c3, 
	(llist_cond_fn_t)entry_cat_cond_eq);
    mu_assert(m_ll->num_nodes == 0, "Entries", 30);
    mu_assert(!m_ll->head, "Entries", 31);
    free_llist(m_ll, (llist_free_data_fn_t)free_nop);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_category(c1);
    free_category(c2);
    free_category(c3);
    return 0;
}


int sort_entry_test() {
    g_entries = init_llist();

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=12, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, NULL, "A Note");
    llist_node_t *en1= init_llist_node(e1);
    llist_insert_to_tail(g_entries, en1);

    // two days newer
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=14, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e2 = init_entry(2, "A Name", date2, -12.00, NULL, "A Note");
    llist_node_t *en2 = init_llist_node(e2);
    llist_insert_to_tail(g_entries, en2);

    // between
    struct tm tm3 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=13, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date3 = mktime(&tm3);
    entry_t *e3 = init_entry(3, "A Name", date3, -12.00, NULL, "A Note");
    llist_node_t *en3 = init_llist_node(e3);
    llist_insert_to_tail(g_entries, en3);

    // earliest
    struct tm tm0 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=11, 
	.tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date0 = mktime(&tm0);

    // change last to earliest (now: en3, en1, en2)
    entry_set_date(e3, date0);
    en3 = llist_sort_node(g_entries, en3, (llist_comp_fn_t)entry_date_comp_gte);
    mu_assert(g_entries->head == en3, "Entries", 32);
    mu_assert(g_entries->head->next == en1, "Entries", 33);
    mu_assert(g_entries->num_nodes == 3, "Entries", 34);

    // change middle to tie for earliest (now: en1, en3, en2)
    entry_set_date(e1, date0);
    en1 = llist_sort_node(g_entries, en1, (llist_comp_fn_t)entry_date_comp_gte);
    mu_assert(g_entries->head == en1, "Entries", 35);
    mu_assert(g_entries->head->next == en3, "Entries", 36);
    mu_assert(en1->next == en3, "Entries", 37);
    mu_assert(en1->prev == NULL, "Entries", 38);
    mu_assert(g_entries->num_nodes == 3, "Entries", 39);

    // change first to latest (now: en3, en2, en1)
    entry_set_date(e1, date2);
    en1 = llist_sort_node(g_entries, en1, (llist_comp_fn_t)entry_date_comp_gte);
    mu_assert(g_entries->head == en3, "Entries", 40);
    mu_assert(g_entries->head->next == en2, "Entries", 41);
    mu_assert(g_entries->tail == en1, "Entries", 42);
    mu_assert(g_entries->num_nodes == 3, "Entries", 43);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);

    return 0;
}
