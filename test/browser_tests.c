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
    g_browser = init_browser(g_entries, 10);
    mu_assert(!g_browser->start, "Browser", 1);
    mu_assert(!g_browser->sel, "Browser", 2);
    mu_assert(!g_browser->end, "Browser", 3);
    free_browser(g_browser);

    // empty el passed
    g_entries = init_llist();
    g_browser = init_browser(g_entries, 10);
    mu_assert(!g_browser->start, "Browser", 4);
    mu_assert(!g_browser->sel, "Browser", 5);
    mu_assert(!g_browser->end, "Browser", 6);
    free_browser(g_browser);
    free_llist(g_entries, free);

    // list with one item but not enough rows to fit it
    g_entries = test_dummy_list(1);

    g_browser = init_browser(g_entries, 0);
    mu_assert(!g_browser->start, "Browser", 7);
    mu_assert(!g_browser->sel, "Browser", 8);
    mu_assert(!g_browser->end, "Browser", 9);
    free_browser(g_browser);

    // list with one item and enough space for it
    g_browser = init_browser(g_entries, 1);
    mu_assert(g_browser->start = g_entries->tail, "Browser", 10);
    mu_assert(g_browser->sel = g_entries->tail, "Browser", 11);
    mu_assert(g_browser->end = g_entries->tail, "Browser", 12);
    free_browser(g_browser);
    free_llist(g_entries, free);

    // two items but only room for 1
    g_entries = test_dummy_list(2);
    g_browser = init_browser(g_entries, 1);
    mu_assert(g_browser->start = g_entries->tail, "Browser", 13);
    mu_assert(g_browser->sel = g_entries->tail, "Browser", 14);
    mu_assert(g_browser->end = g_entries->tail, "Browser", 15);
    free_browser(g_browser);
    
    // two items with room for both 
    g_browser = init_browser(g_entries, 2);
    mu_assert(g_browser->start = g_entries->head, "Browser", 16);
    mu_assert(g_browser->sel = g_entries->tail, "Browser", 17);
    mu_assert(g_browser->end = g_entries->tail, "Browser", 18);
    free_browser(g_browser);
    free_llist(g_entries, free);

    return 0;
}


int browser_scroll_test() {
    g_entries = test_dummy_list(3);

    // room for full list
    g_browser = init_browser(g_entries, 6);
    
    // can't scroll down
    browser_scroll(1, DOWN); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 19);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 20);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 21);

    browser_scroll(1, UP); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 22);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 23);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 24);

    // scroll beyond head
    browser_scroll(2, UP); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 25);
    mu_assert(g_browser->sel == g_entries->head, "Browser", 26);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 27);

    // back to bottom
    browser_scroll(2, DOWN); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 28);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 29);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 30);

    free_browser(g_browser);

    // only room for bottom 2 in g_entries
    g_browser = init_browser(g_entries, 2);

    browser_scroll(1, UP); 
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 31);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 32);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 33);

    // one more up changes start end context
    browser_scroll(1, UP); 
    mu_assert(g_browser->start == g_entries->head, "Browser", 34);
    mu_assert(g_browser->sel == g_entries->head, "Browser", 35);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 36);

    browser_scroll(2, DOWN); 
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 37);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 38);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 39);

    free_llist(g_entries, free);
    free_browser(g_browser);

    return 0;
}


int browser_append_to_tail_test() {

    // empty_list
    g_entries = init_llist();
    // browser with room for more entries
    g_browser = init_browser(g_entries, 10);

    llist_node_t *en0 = init_llist_node(NULL);
    llist_insert_node(g_entries, en0, llist_to_tail, NULL);
    browser_insert(en0);
    
    mu_assert(g_browser->end == en0, "Browser", 40);
    mu_assert(g_browser->sel == en0, "Browser", 41);
    mu_assert(g_browser->start == en0, "Browser", 42);
    mu_assert(g_browser->num_entries == 1, "Browser", 43);


    // append while at tail
    llist_node_t *en1 = init_llist_node(NULL);
    llist_insert_node(g_entries, en1, llist_to_tail, NULL);
    browser_insert(en1);

    mu_assert(g_browser->end == en1, "Browser", 44);
    mu_assert(g_browser->sel == en1, "Browser", 45);
    mu_assert(g_browser->start == g_entries->head, "Browser", 46);
    mu_assert(g_browser->num_entries == 2, "Browser", 47);

    // append while not at tail i.e. go to tail
    browser_scroll(2, UP); 
    llist_node_t *en2 = init_llist_node(NULL);
    llist_insert_node(g_entries, en2, llist_to_tail, NULL);
    browser_insert(en2);

    mu_assert(g_browser->end == en2, "Browser", 48);
    mu_assert(g_browser->sel == en2, "Browser", 49);
    mu_assert(g_browser->start == g_entries->head, "Browser", 50);
    mu_assert(g_browser->num_entries == 3, "Browser", 51);
    
    free_browser(g_browser);
    
    // browser smaller than num entries
    g_browser = init_browser(g_entries, 3);

    llist_node_t *en3 = init_llist_node(NULL);
    llist_insert_node(g_entries, en3, llist_to_tail, NULL);
    llist_node_t *temp = g_browser->start;

    // append while at tail 
    browser_insert(en3);

    mu_assert(g_browser->end == en3, "Browser", 52);
    mu_assert(g_browser->sel == en3, "Browser", 53);
    mu_assert(g_browser->start == temp->next, "Browser", 54);
    // num_entries in browser does not change (still full)
    mu_assert(g_browser->num_entries == 3, "Browser", 55);

    // move all context above tail
    browser_scroll(3, UP); 
    llist_node_t *en4 = init_llist_node(NULL);
    llist_insert_node(g_entries, en4, llist_to_tail, NULL);
    browser_insert(en4);

    mu_assert(g_browser->end == en4, "Browser", 56);
    mu_assert(g_browser->sel == en4, "Browser", 57);
    mu_assert(g_browser->start == g_entries->tail->prev->prev, "Browser", 58);
    mu_assert(g_browser->num_entries == 3, "Browser", 59);

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

    g_browser = init_browser(g_entries, 12);

    // delete middle
    // move selected to middle
    llist_node_traverse(&g_browser->sel, UP);
    // delete selected 
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->head->next, "Browser", 60);
    mu_assert(g_browser->sel == g_entries->tail, "Browser", 61);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 62);
    mu_assert(g_browser->start == g_entries->head, "Browser", 63);
    mu_assert(g_browser->num_entries == 2, "Browser", 64);

    // reset browser
    llist_node_traverse(&g_browser->sel, DOWN);
    llist_node_traverse(&g_browser->end, DOWN);
    g_browser->num_entries++;
    // el tail
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->tail, "Browser", 65);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 66);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 67);
    mu_assert(g_browser->start == g_entries->head, "Browser", 68);
    mu_assert(g_browser->num_entries == 2, "Browser", 69);

    // reset browser
    llist_node_traverse(&g_browser->end, DOWN);
    llist_node_traverse(&g_browser->sel, DOWN);
    (g_browser->num_entries)++;
    // move to head and delete head
    llist_node_traverse(&g_browser->sel, UP);
    llist_node_traverse(&g_browser->sel, UP);
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->head, "Browser", 70);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 71);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 72);
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 73);
    mu_assert(g_browser->num_entries == 2, "Browser", 74);

    free_browser(g_browser);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------
    
    // not enough space for one of three nodes
    g_browser = init_browser(g_entries, 2);

    // delete tail
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->tail, "Browser", 75);
    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 76);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 77);
    mu_assert(g_browser->start == g_entries->head, "Browser", 78);
    mu_assert(g_browser->num_entries == 2, "Browser", 79);

    // reset browser
    llist_node_traverse(&g_browser->sel, DOWN);
    llist_node_traverse(&g_browser->start, DOWN);
    llist_node_traverse(&g_browser->end, DOWN);
    // move up and delete non-tail
    llist_node_traverse(&g_browser->sel, UP);
    popped_en = browser_pop_sel_entry();

    mu_assert(popped_en == g_entries->tail->prev, "Browser", 80);
    mu_assert(g_browser->sel == g_entries->head, "Browser", 81);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 82);
    mu_assert(g_browser->start == g_entries->head, "Browser", 83);
    mu_assert(g_browser->num_entries == 2, "Browser", 84);

    free_llist(g_entries, free);
    free_browser(g_browser);
    
    // ------------------------------------------------------------------------
    // Delete from empty list
    // ------------------------------------------------------------------------

    g_entries = init_llist();
    g_browser = init_browser(g_entries, 2);

    mu_assert(g_browser->sel == NULL, "Browser", 85);
    mu_assert(g_browser->end == NULL, "Browser", 86);
    mu_assert(g_browser->start == NULL, "Browser", 87);
    mu_assert(g_browser->num_entries == 0, "Browser", 88);

    free_llist(g_entries, free);
    free_browser(g_browser);
    return 0;
}


int browser_insert_after_date_test() {

    g_entries = init_llist();
    // browser with room for more entries
    g_browser = init_browser(g_entries, 3);

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=12, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, 1, "A Note");
    llist_node_t *en1= init_llist_node(e1);
    llist_insert_node(g_entries, en1, llist_after_node,
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en1);

    mu_assert(g_browser->end == en1, "Browser", 92);
    mu_assert(g_browser->sel == en1, "Browser", 93);
    mu_assert(g_browser->start == en1, "Browser", 94);
    mu_assert(g_browser->num_entries == 1, "Browser", 95);

    // earlier date
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=1, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e2 = init_entry(2, "A Name", date2, -12.00, 1, "A Note");
    llist_node_t *en2= init_llist_node(e2);
    llist_insert_node(g_entries, en2, llist_after_node,
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en2);

    mu_assert(g_browser->sel == en2, "Browser", 96);
    mu_assert(g_browser->end == en1, "Browser", 97);
    mu_assert(g_browser->start == en2, "Browser", 98);
    mu_assert(g_browser->num_entries == 2, "Browser", 99);

    // between the two
    struct tm tm3 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=4, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date3 = mktime(&tm3);
    entry_t *e3 = init_entry(3, "A Name", date3, -12.00, 1, "A Note");
    llist_node_t *en3= init_llist_node(e3);
    llist_insert_node(g_entries, en3, llist_after_node,
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en3);

    mu_assert(g_browser->sel == en3, "Browser", 100);
    mu_assert(g_browser->end == en1, "Browser", 101);
    mu_assert(g_browser->start == en2, "Browser", 102);
    mu_assert(g_browser->num_entries == 3, "Browser", 103);

    // new en at where start was
    struct tm tm4 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=3, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date4 = mktime(&tm4);
    entry_t *e4 = init_entry(4, "A Name", date4, -12.00, 1, "A Note");
    llist_node_t *en4= init_llist_node(e4);
    llist_insert_node(g_entries, en4, llist_after_node,
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en4);

    // en2, en4, en3, en1
    mu_assert(g_browser->sel == en4, "Browser", 104);
    mu_assert(g_browser->end == en1, "Browser", 105);
    mu_assert(g_browser->start == en4, "Browser", 106);
    mu_assert(g_browser->num_entries == 3, "Browser", 107);

    // new en at where end was
    struct tm tm5 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=9, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date5 = mktime(&tm5);
    entry_t *e5 = init_entry(5, "A Name", date5, -12.00, 1, "A Note");
    llist_node_t *en5= init_llist_node(e5);
    llist_insert_node(g_entries, en5, llist_after_node,
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en5);

    // en2, en4, en3, en5, en1
    mu_assert(g_browser->sel == en5, "Browser", 108);
    mu_assert(g_browser->end == en1, "Browser", 109);
    mu_assert(g_browser->start == en3, "Browser", 110);
    mu_assert(g_browser->num_entries == 3, "Browser", 111);

    // new en at head (tail already covered in `append_llist_to_tail, NULL` tests)
    // while not at head
    struct tm tm6 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=29, .tm_mon=11, .tm_year=2021 - 1900, .tm_isdst=1}; 
    time_t date6 = mktime(&tm6);
    entry_t *e6 = init_entry(6, "A Name", date6, -12.00, 1, "A Note");
    llist_node_t *en6= init_llist_node(e6);
    llist_insert_node(g_entries, en6, llist_after_node,
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en6);

    // en6, en2, en4, en3, en5, en1
    mu_assert(g_browser->sel == en6, "Browser", 112);
    mu_assert(g_browser->end == en4, "Browser", 113);
    mu_assert(g_browser->start == en6, "Browser", 114);
    mu_assert(g_browser->num_entries == 3, "Browser", 115);

    // new en at head (tail already covered in `append_llist_to_tail, NULL` tests)
    // while at head
    struct tm tm9 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=29, .tm_mon=10, .tm_year=2021 - 1900, .tm_isdst=1}; 
    time_t date9 = mktime(&tm9);
    entry_t *e9 = init_entry(9, "A Name", date9, -12.00, 1, "A Note");
    llist_node_t *en9= init_llist_node(e9);
    llist_insert_node(g_entries, en9, llist_after_node, 
	    (llist_comp_fn_t)entry_date_comp_gte);
    browser_insert(en9);

    // en9, en6, en2, en4, en3, en5, en1
    mu_assert(g_browser->sel == en9, "Browser", 116);
    mu_assert(g_browser->end == en2, "Browser", 117);
    mu_assert(g_browser->start == en9, "Browser", 118);
    mu_assert(g_browser->num_entries == 3, "Browser", 119);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_browser(g_browser);

    return 0;
}
