#include "test.h"


int browser_tests() {
    mu_run_test(browser_init_test);
    mu_run_test(browser_scroll_test);
    mu_run_test(browser_del_entry_test);
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
    g_entries = init_entry_list();
    g_browser = init_browser(g_entries, 10);
    mu_assert(!g_browser->start, "Browser", 4);
    mu_assert(!g_browser->sel, "Browser", 5);
    mu_assert(!g_browser->end, "Browser", 6);
    free_browser(g_browser);
    free_entry_list(g_entries);

    // list with one item but not enough rows to fit it
    g_entries = test_dummy_entry_list(1);

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
    free_entry_list(g_entries);

    // two items but only room for 1
    g_entries = test_dummy_entry_list(2);
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
    free_entry_list(g_entries);

    return 0;
}


int browser_scroll_test() {
    g_entries = test_dummy_entry_list(3);

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

    free_entry_list(g_entries);
    free_browser(g_browser);

    return 0;
}


int browser_del_entry_test() {
    // For this series of tests, only delete from browser (i.e. move pointers),
    // but do not actually delete the entries from the list or db (they're not
    // written there anyway)
    
    g_entries = test_dummy_entry_list(3);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------

    g_browser = init_browser(g_entries, 12);

    // delete middle
    // move selected to middle
    entry_node_traverse(&g_browser->sel, UP);
    // delete selected 
    browser_del_entry();

    mu_assert(g_browser->sel == g_entries->tail, "Browser", 40);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 41);
    mu_assert(g_browser->start == g_entries->head, "Browser", 42);
    mu_assert(g_browser->num_entries == 2, "Browser", 43);

    // reset browser
    entry_node_traverse(&g_browser->sel, DOWN);
    entry_node_traverse(&g_browser->end, DOWN);
    g_browser->num_entries++;
    // el tail
    browser_del_entry();

    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 44);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 45);
    mu_assert(g_browser->start == g_entries->head, "Browser", 46);
    mu_assert(g_browser->num_entries == 2, "Browser", 47);

    // reset browser
    entry_node_traverse(&g_browser->end, DOWN);
    entry_node_traverse(&g_browser->sel, DOWN);
    (g_browser->num_entries)++;
    // move to head and delete head
    entry_node_traverse(&g_browser->sel, UP);
    entry_node_traverse(&g_browser->sel, UP);
    browser_del_entry();

    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 48);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 49);
    mu_assert(g_browser->start == g_entries->head->next, "Browser", 50);
    mu_assert(g_browser->num_entries == 2, "Browser", 51);

    free_browser(g_browser);

    // ------------------------------------------------------------------------
    // Browser size > num nodes
    // ------------------------------------------------------------------------
    
    // not enough space for one of three nodes
    g_browser = init_browser(g_entries, 2);

    // delete tail
    browser_del_entry();

    mu_assert(g_browser->sel == g_entries->head->next, "Browser", 52);
    mu_assert(g_browser->end == g_entries->head->next, "Browser", 53);
    mu_assert(g_browser->start == g_entries->head, "Browser", 54);
    mu_assert(g_browser->num_entries == 2, "Browser", 55);

    // reset browser
    entry_node_traverse(&g_browser->sel, DOWN);
    entry_node_traverse(&g_browser->start, DOWN);
    entry_node_traverse(&g_browser->end, DOWN);
    // move up and delete non-tail
    entry_node_traverse(&g_browser->sel, UP);
    browser_del_entry();

    mu_assert(g_browser->sel == g_entries->head, "Browser", 56);
    mu_assert(g_browser->end == g_entries->tail, "Browser", 57);
    mu_assert(g_browser->start == g_entries->head, "Browser", 58);
    mu_assert(g_browser->num_entries == 2, "Browser", 59);

    free_entry_list(g_entries);
    free_browser(g_browser);
    
    // ------------------------------------------------------------------------
    // Delete from empty list
    // ------------------------------------------------------------------------

    g_entries = init_entry_list();
    g_browser = init_browser(g_entries, 2);

    mu_assert(g_browser->sel == NULL, "Browser", 60);
    mu_assert(g_browser->end == NULL, "Browser", 61);
    mu_assert(g_browser->start == NULL, "Browser", 62);
    mu_assert(g_browser->num_entries == 0, "Browser", 63);

    free_entry_list(g_entries);
    free_browser(g_browser);
    return 0;
}
