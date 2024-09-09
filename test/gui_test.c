#include "test.h"


void test_gui() {
    initscr();
    curs_set(0);

    int max_row = getmaxy(stdscr);
    entry_list_t* el = init_entry_list();
    browser_buf_t* bb = init_browser_buf(el, max_row);

    // empty draw
    draw_browser(stdscr, bb);
    getch();

    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    // max negative value
    entry_t* test_entry1 = init_entry("Starbucks", test_date1, -99999.99, "Food",
            "Cafe", "Zionist scum!");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    entry_list_push(el, test_entry_node1);
    // positive w/out subcat
    entry_t* test_entry2 = init_entry("Starbucks", test_date1, 1750.67, "Food",
            NULL, "Zionist scum!");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    entry_list_push(el, test_entry_node2);
    // tiny 
    entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, "Food",
            NULL, "Zionist scum!");
    entry_node_t* test_entry_node3 = init_entry_node(test_entry3);
    entry_list_push(el, test_entry_node3);
    // long name to be truncated
    entry_t* test_entry4 = init_entry("Starbucks and some extra text", test_date1, 
             -0.23, "Food", NULL, "Zionist scum!");
    entry_node_t* test_entry_node4 = init_entry_node(test_entry4);
    entry_list_push(el, test_entry_node4);

    free_browser_buf(bb);
    bb = init_browser_buf(el, max_row);

    draw_browser(stdscr, bb);
    getch();

    // many
    for (int i = 0; i < 12; i++) {
        entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, "Food",
            NULL, "Zionist scum!");
	entry_node_t* test_entry_node3 = init_entry_node(test_entry3);
	entry_list_push(el, test_entry_node3);
    }

    free_browser_buf(bb);
    bb = init_browser_buf(el, max_row);
    draw_browser(stdscr, bb);
    getch();

    free_browser_buf(bb);
    free_entry_list(el);

    endwin();
}
