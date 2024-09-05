#include "test.h"


void test_gui() {
    initscr();
    curs_set(0);

    entry_list_t* entries = init_entry_list();

    // empty draw
    draw_browser(stdscr, entries, entries->tail);

    struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t test_date1 = mktime(&test_time_tm1);
    // max negative value
    entry_t* test_entry1 = init_entry("Starbucks", test_date1, -99999.99, "Food",
	    "Cafe", "Zionist scum!");
    append_to_tail(entries, test_entry1);
    // positive w/out subcat
    entry_t* test_entry2 = init_entry("Starbucks", test_date1, 1750.67, "Food",
	    NULL, "Zionist scum!");
    append_to_tail(entries, test_entry2);
    // tiny 
    entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, "Food",
	    NULL, "Zionist scum!");
    append_to_tail(entries, test_entry3);

    // long name to be truncated
    entry_t* test_entry4 = init_entry("Starbucks and some extra text", test_date1, 
             -0.23, "Food", NULL, "Zionist scum!");
    append_to_tail(entries, test_entry4);

    draw_browser(stdscr, entries, entries->tail);
    getch();

    // many
    for (int i = 0; i < 12; i++) {
	entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, "Food",
	    NULL, "Zionist scum!");
	append_to_tail(entries, test_entry3);
    }

    draw_browser(stdscr, entries, entries->tail);
    getch();

    free_list(entries);


    endwin();
}
