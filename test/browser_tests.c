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


int browser_tests() {
    mu_run_test(browser_init_test);
    mu_run_test(browser_scroll_test);
    mu_run_test(browser_append_to_tail_test);
    mu_run_test(browser_pop_sel_entry_test);
    mu_run_test(browser_insert_after_date_test);
    return 0;
}


int browser_init_test() {

    // NULL el passed
    g_entries = NULL;
    g_browser = init_browser(g_entries, NULL, 0, 10);
    mu_assert(!g_browser->start, "Browser", 1);
    mu_assert(!g_browser->sel, "Browser", 2);
    mu_assert(!g_browser->end, "Browser", 3);
    free_browser(g_browser);

    // empty el passed
    g_entries = init_llist();
    g_browser = init_browser(g_entries, g_entries->tail, 0, 10);
    mu_assert(!g_browser->start, "Browser", 4);
    mu_assert(!g_browser->sel, "Browser", 5);
    mu_assert(!g_browser->end, "Browser", 6);
    free_browser(g_browser);
    free_llist(g_entries, free);

    // list with one item but not enough rows to fit it
    g_entries = test_dummy_list(1);

    g_browser = init_browser(g_entries, g_entries->tail, 0, 0);
    mu_assert(!g_browser->start, "Browser", 7);
    mu_assert(!g_browser->sel, "Browser", 8);
    mu_assert(!g_browser->end, "Browser", 9);
    free_browser(g_browser);

    // list with one item and enough space for it
    g_browser = init_browser(g_entries, g_entries->tail, 0, 1);
    mu_assert(g_browser->start == g_entries->tail, "Browser", 10);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 11);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 12);
    free_browser(g_browser);
    free_llist(g_entries, free);

    // two items but only room for 1
    g_entries = test_dummy_list(2);
    g_browser = init_browser(g_entries, g_entries->tail, 0, 1);
    mu_assert(g_browser->start == g_entries->tail, "Browser", 13);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 14);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 15);
    free_browser(g_browser);
    
    // two items with room for both 
    g_browser = init_browser(g_entries, g_entries->tail, 0, 2);
    mu_assert(g_browser->start == g_entries->head, "Browser", 16);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 17);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 18);
    free_browser(g_browser);
    free_llist(g_entries, free);

    // end and start are both out of context and ask for sel to be in middle of
    // browser by choosing `-1` option
    g_entries = test_dummy_list(20);
    llist_node_t *cur = g_entries->tail;
    for (int i = 0; i < 10; i++) 
	cur = cur->prev;

    g_browser = init_browser(g_entries, cur, -1, 5);
    mu_assert(g_browser->start == cur->prev->prev, "Browser", 19);
    mu_assert(g_browser->sel == cur, "Browser", 20);
    mu_assert(g_browser->end == cur->next->next, "Browser", 21);
    free_browser(g_browser);

    // again but with even browser slots
    g_browser = init_browser(g_entries, cur, -1, 4);
    mu_assert(g_browser->start = cur->prev->prev, "Browser", 22);
    mu_assert(g_browser->sel == cur, "Browser", 23);
    mu_assert(g_browser->end == cur->next, "Browser", 24);
    free_browser(g_browser);

    // sel predefined to be 2 above tail (i.e. not default middle/preserving space)
    // but both tail and head out of context
    g_browser = init_browser(g_entries, cur, 2, 7);
    mu_assert(g_browser->start == cur->prev->prev->prev->prev, 
	"Browser", 25);
    mu_assert(g_browser->sel == cur, "Browser", 26);
    mu_assert(g_browser->end == cur->next->next, "Browser", 27);
    free_browser(g_browser);

    // middle but both tail and head are IN context
    g_browser = init_browser(g_entries, cur, -1, 22);
    mu_assert(g_browser->start == g_entries->head, "Browser", 28);
    mu_assert(g_browser->sel == cur, "Browser", 29);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 30);
    free_browser(g_browser);

    // ideally 2 down but since cur is in middle, won't be possible.
    g_browser = init_browser(g_entries, cur, 2, 22);
    mu_assert(g_browser->start == g_entries->head, "Browser", 31);
    mu_assert(g_browser->sel == cur, "Browser", 32);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 33);
    free_browser(g_browser);

    // ideally 2 from top but since cur is in middle, won't be possible.
    g_browser = init_browser(g_entries, cur, 18, 22);
    mu_assert(g_browser->start == g_entries->head, "Browser", 31);
    mu_assert(g_browser->sel == cur, "Browser", 32);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 33);
    mu_assert(g_browser->num_entries == 20, "Browser", 34);

    free_browser(g_browser);
    free_llist(g_entries, free);

    return 0;
}


int browser_scroll_test() {
    g_entries = test_dummy_list(3);

    // room for full list
    g_browser = init_browser(g_entries, g_entries->tail, 0, 6);
    
    // can't scroll down
    browser_scroll(1, DOWN); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 35);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 36);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 37);

    browser_scroll(1, UP); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 38);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 39);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 40);

    // scroll beyond head
    browser_scroll(2, UP); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 41);
    mu_assert(g_browser->sel == g_entries->head, "Browser", 42);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 43);

    // back to bottom
    browser_scroll(2, DOWN); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 44);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 45);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 46);

    free_browser(g_browser);

    // only room for bottom 2 in g_entries
    g_browser = init_browser(g_entries, g_entries->tail, 0, 2);

    browser_scroll(1, UP); 
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 47);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 48);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 49);

    // one more up changes start end context
    browser_scroll(1, UP); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 50);
    mu_assert(g_browser->sel == g_entries->head, "Browser", 51);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 52);

    browser_scroll(2, DOWN); 
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 53);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 54);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 55);

    free_llist(g_entries, free);
    free_browser(g_browser);

    return 0;
}


int browser_append_to_tail_test() {

    // empty_list
    g_entries = init_llist();
    // browser with room for more entries
    g_browser = init_browser(g_entries, g_entries->tail, 0, 10);

    llist_node_t *en0 = init_llist_node(NULL);
    llist_insert_to_tail(g_entries, en0);
    browser_insert(en0);
    
    mu_assert(g_browser->end == en0, "Browser", 56);
    mu_assert(g_browser->sel == en0, "Browser", 57);
    mu_assert(g_browser->start == en0, "Browser", 58);
    mu_assert(g_browser->num_entries == 1, "Browser", 59);


    // append while at tail
    llist_node_t *en1 = init_llist_node(NULL);
    llist_insert_to_tail(g_entries, en1);
    browser_insert(en1);

    mu_assert(g_browser->end == en1, "Browser", 60);
    mu_assert(g_browser->sel == en1, "Browser", 61);
    mu_assert(g_browser->start == g_entries->head, "Browser", 62);
    mu_assert(g_browser->num_entries == 2, "Browser", 63);

    // append while not at tail i.e. go to tail
    browser_scroll(2, UP); 
    llist_node_t *en2 = init_llist_node(NULL);
    llist_insert_to_tail(g_entries, en2);
    browser_insert(en2);

    mu_assert(g_browser->end == en2, "Browser", 64);
    mu_assert(g_browser->sel == en2, "Browser", 65);
    mu_assert(g_browser->start == g_entries->head, "Browser", 66);
    mu_assert(g_browser->num_entries == 3, "Browser", 67);
    
    free_browser(g_browser);
    
    // browser smaller than num entries
    g_browser = init_browser(g_entries, g_entries->tail, 0, 3);

    llist_node_t *en3 = init_llist_node(NULL);
    llist_insert_to_tail(g_entries, en3);
    llist_node_t *temp = g_browser->start;

    // append while at tail 
    browser_insert(en3);

    mu_assert(g_browser->end == en3, "Browser", 68);
    mu_assert(g_browser->sel == en3, "Browser", 69);
    mu_assert(g_browser->start == temp->next, "Browser", 70);
    // num_entries in browser does not change (still full)
    mu_assert(g_browser->num_entries == 3, "Browser", 71);

    // move all context above tail
    browser_scroll(3, UP); 
    llist_node_t *en4 = init_llist_node(NULL);
    llist_insert_to_tail(g_entries, en4);
    browser_insert(en4);

    mu_assert(g_browser->end == en4, "Browser", 72);
    mu_assert(g_browser->sel == en4, "Browser", 73);
    mu_assert(g_browser->start == g_entries->tail->prev->prev, "Browser", 74);
    mu_assert(g_browser->num_entries == 3, "Browser", 75);

    free_llist(g_entries, free);
    free_browser(g_browser);

    return 0;
}


int browser_pop_sel_entry_test() {
    // For this series of tests, only delete from browser (i.e. move pointers),
    // but do not actually delete the entries from the list or db (they're not
    // written there anyway)
    
    llist_node_t *popped_en = NULL;
    g_entries = test_dummy_list(3);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------

    g_browser = init_browser(g_entries, g_entries->tail, 0, 12);

    // delete middle
    // move selected to middle
    llist_node_traverse(&g_browser->sel, UP);
    // delete selected 
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->head->next, "Browser", 76);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 77);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 78);
    mu_assert(g_browser->start == g_entries->head, "Browser", 79);
    mu_assert(g_browser->num_entries == 2, "Browser", 80);

    // reset browser
    llist_node_traverse(&g_browser->sel, DOWN);
    llist_node_traverse(&g_browser->end, DOWN);
    g_browser->num_entries++;
    // el tail
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->tail, "Browser", 81);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 82);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 83);
    mu_assert(g_browser->start == g_entries->head, "Browser", 84);
    mu_assert(g_browser->num_entries == 2, "Browser", 85);

    // reset browser
    llist_node_traverse(&g_browser->end, DOWN);
    llist_node_traverse(&g_browser->sel, DOWN);
    (g_browser->num_entries)++;
    // move to head and delete head
    llist_node_traverse(&g_browser->sel, UP);
    llist_node_traverse(&g_browser->sel, UP);
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->head, "Browser", 86);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 87);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 88);
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 89);
    mu_assert(g_browser->num_entries == 2, "Browser", 90);

    free_browser(g_browser);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------
    
    // not enough space for one of three nodes
    g_browser = init_browser(g_entries, g_entries->tail, 0, 2);

    // delete tail
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->tail, "Browser", 91);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 92);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 93);
    mu_assert(g_browser->start == g_entries->head, "Browser", 94);
    mu_assert(g_browser->num_entries == 2, "Browser", 95);

    // reset browser
    llist_node_traverse(&g_browser->sel, DOWN);
    llist_node_traverse(&g_browser->start, DOWN);
    llist_node_traverse(&g_browser->end, DOWN);
    // move up and delete non-tail
    llist_node_traverse(&g_browser->sel, UP);
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->tail->prev, "Browser", 96);
    mu_assert(g_browser->sel == g_entries->head, "Browser", 97);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 98);
    mu_assert(g_browser->start == g_entries->head, "Browser", 99);
    mu_assert(g_browser->num_entries == 2, "Browser", 100);

    free_llist(g_entries, free);
    free_browser(g_browser);
    
    // ------------------------------------------------------------------------
    // Delete from empty list
    // ------------------------------------------------------------------------

    g_entries = init_llist();
    g_browser = init_browser(g_entries, g_entries->tail, 0, 2);

    mu_assert(g_browser->sel == NULL, "Browser", 101);
    mu_assert(g_browser->end == NULL, "Browser", 102);
    mu_assert(g_browser->start == NULL, "Browser", 103);
    mu_assert(g_browser->num_entries == 0, "Browser", 104);

    free_llist(g_entries, free);
    free_browser(g_browser);
    return 0;
}


int browser_insert_after_date_test() {

    category_t *tc = init_category(1, 0, "Food");
    g_entries = init_llist();
    // browser with room for more entries
    g_browser = init_browser(g_entries, g_entries->tail, 0, 3);

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=12, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, tc, "A Note");
    llist_node_t *en1= init_llist_node(e1);
    llist_insert_node(g_entries, en1, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en1);

    mu_assert(g_browser->end == en1, "Browser", 108);
    mu_assert(g_browser->sel == en1, "Browser", 109);
    mu_assert(g_browser->start == en1, "Browser", 110);
    mu_assert(g_browser->num_entries == 1, "Browser", 111);

    // earlier date
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=1, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e2 = init_entry(2, "A Name", date2, -12.00, tc, "A Note");
    llist_node_t *en2= init_llist_node(e2);
    llist_insert_node(g_entries, en2, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en2);

    mu_assert(g_browser->sel == en2, "Browser", 112);
    mu_assert(g_browser->end == en1, "Browser", 113);
    mu_assert(g_browser->start == en2, "Browser", 114);
    mu_assert(g_browser->num_entries == 2, "Browser", 115);

    // between the two
    struct tm tm3 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=4, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date3 = mktime(&tm3);
    entry_t *e3 = init_entry(3, "A Name", date3, -12.00, tc, "A Note");
    llist_node_t *en3= init_llist_node(e3);
    llist_insert_node(g_entries, en3, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en3);

    mu_assert(g_browser->sel == en3, "Browser", 116);
    mu_assert(g_browser->end == en1, "Browser", 117);
    mu_assert(g_browser->start == en2, "Browser", 118);
    mu_assert(g_browser->num_entries == 3, "Browser", 119);

    // new en at where start was
    struct tm tm4 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=3, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date4 = mktime(&tm4);
    entry_t *e4 = init_entry(4, "A Name", date4, -12.00, tc, "A Note");
    llist_node_t *en4= init_llist_node(e4);
    llist_insert_node(g_entries, en4, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en4);

    // en2, en4, en3, en1
    mu_assert(g_browser->sel == en4, "Browser", 120);
    mu_assert(g_browser->end == en1, "Browser", 121);
    mu_assert(g_browser->start == en4, "Browser", 122);
    mu_assert(g_browser->num_entries == 3, "Browser", 123);

    // new en at where end was
    struct tm tm5 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=9, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date5 = mktime(&tm5);
    entry_t *e5 = init_entry(5, "A Name", date5, -12.00, tc, "A Note");
    llist_node_t *en5= init_llist_node(e5);
    llist_insert_node(g_entries, en5, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en5);

    // en2, en4, en3, en5, en1
    mu_assert(g_browser->sel == en5, "Browser", 124);
    mu_assert(g_browser->end == en1, "Browser", 125);
    mu_assert(g_browser->start == en3, "Browser", 126);
    mu_assert(g_browser->num_entries == 3, "Browser", 127);

    // new en at head (tail already covered in `append_llist_to_tail, NULL` tests)
    // while not at head
    struct tm tm6 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=29, .tm_mon=11, .tm_year=2021 - 1900, .tm_isdst=1}; 
    time_t date6 = mktime(&tm6);
    entry_t *e6 = init_entry(6, "A Name", date6, -12.00, tc, "A Note");
    llist_node_t *en6= init_llist_node(e6);
    llist_insert_node(g_entries, en6, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en6);

    // en6, en2, en4, en3, en5, en1
    mu_assert(g_browser->sel == en6, "Browser", 128);
    mu_assert(g_browser->end == en4, "Browser", 129);
    mu_assert(g_browser->start == en6, "Browser", 130);
    mu_assert(g_browser->num_entries == 3, "Browser", 131);

    // new en at head (tail already covered in `append_llist_to_tail, NULL` tests)
    // while at head
    struct tm tm9 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
       .tm_mday=29, .tm_mon=10, .tm_year=2021 - 1900, .tm_isdst=1}; 
    time_t date9 = mktime(&tm9);
    entry_t *e9 = init_entry(9, "A Name", date9, -12.00, tc, "A Note");
    llist_node_t *en9= init_llist_node(e9);
    llist_insert_node(g_entries, en9, (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en9);

    // en9, en6, en2, en4, en3, en5, en1
    mu_assert(g_browser->sel == en9, "Browser", 132);
    mu_assert(g_browser->end == en2, "Browser", 133);
    mu_assert(g_browser->start == en9, "Browser", 134);
    mu_assert(g_browser->num_entries == 3, "Browser", 135);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_browser(g_browser);
    free_category(tc);

    return 0;
}
