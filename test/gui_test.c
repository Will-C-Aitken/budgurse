#include "test.h"


void test_gui() {
    initscr();
    curs_set(0);

    int max_row = getmaxy(stdscr);
    entry_list_t* el = init_entry_list();
    browser_t* b = init_browser(el, max_row);

    // empty draw
    draw_browser(stdscr, b);
    getch();

    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    // max negative value
    entry_t* test_entry1 = init_entry("Starbucks", test_date1, -99999.99, "Food",
            "Cafe", "Zionist scum!");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(el, test_entry_node1);
    // positive w/out subcat
    entry_t* test_entry2 = init_entry("Starbucks", test_date1, 1750.67, "Food",
            NULL, "Zionist scum!");
    entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
    append_to_tail(el, test_entry_node2);
    // tiny 
    entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, "Food",
            NULL, "Zionist scum!");
    entry_node_t* test_entry_node3 = init_entry_node(test_entry3);
    append_to_tail(el, test_entry_node3);
    // long name to be truncated
    entry_t* test_entry4 = init_entry("Starbucks and some extra text", test_date1, 
             -0.23, "Food", NULL, "Zionist scum!");
    entry_node_t* test_entry_node4 = init_entry_node(test_entry4);
    append_to_tail(el, test_entry_node4);

    free_browser(b);
    b = init_browser(el, max_row);

    int result, ch;
    do {
	draw_browser(stdscr, b);
	ch = getch();
	result = browser_handle_key(b, ch);
    } while(result);
    free_browser(b);

    // many
    for (int i = 0; i < 12; i++) {
        entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, "Food",
            NULL, "Zionist scum!");
        entry_node_t* test_entry_node3 = init_entry_node(test_entry3);
        append_to_tail(el, test_entry_node3);
    }

    b = init_browser(el, max_row);
    do {
	draw_browser(stdscr, b);
	ch = getch();
	result = browser_handle_key(b, ch);
    } while(result);

    free_browser(b);

    free_entry_list(el);

    endwin();
}
