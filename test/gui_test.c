#include "test.h"

entry_list_t *entries;
cat_array_t *categories;
sqlite3 *db;
browser_t *browser;
win_t wins[NUM_WINS];

state_t state;

void test_gui() {

    init_budgurse();
    draw_browser();

    while(1) {
	int ch = wgetch(wins[state].win);
	if (!handle_input(ch))
	    break;
    }
//     // prepare category array
//     cat_array_t *ca = init_cat_array();
//     category_t *c1 = init_category(1, 0, "Food");
//     append_to_cat_array(ca, c1);
//     category_t *c2 = init_category(2, 1, "Cafe");
//     append_to_cat_array(ca, c2);
// 
//     entry_list_t* el = init_entry_list();
//     browser_t* b = init_browser(el, -1);
//     prompt_t* p = init_prompt();
// 
//     // empty draw
//     draw_browser(b, ca);
//     prompt_new_entry(p, el, ca);
//     wgetch(b->win);
// 
//     struct tm test_time_tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
//     time_t test_date1 = mktime(&test_time_tm1);
//     // max negative value
//     entry_t* test_entry1 = init_entry("Starbucks", test_date1, -99999.99, 2,
//             "Zionist scum!");
//     entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
//     append_to_tail(el, test_entry_node1);
//     // positive w/out subcat
//     entry_t* test_entry2 = init_entry("Starbucks", test_date1, 1750.67, 1,
//             "Zionist scum!");
//     entry_node_t* test_entry_node2 = init_entry_node(test_entry2);
//     append_to_tail(el, test_entry_node2);
//     // tiny 
//     entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, 1,
//              "Zionist scum!");
//     entry_node_t* test_entry_node3 = init_entry_node(test_entry3);
//     append_to_tail(el, test_entry_node3);
//     // long name to be truncated
//     entry_t* test_entry4 = init_entry("Starbucks and some extra text", test_date1, 
//              -0.23, 1, "Zionist scum!");
//     entry_node_t* test_entry_node4 = init_entry_node(test_entry4);
//     append_to_tail(el, test_entry_node4);
// 
//     free_browser(b);
//     b = init_browser(el, -1);
// 
//     int result, ch;
//     do {
// 	draw_browser(b, ca);
// 	ch = wgetch(b->win);
// 	result = browser_handle_key(b, ch);
//     } while(result);
//     free_browser(b);
// 
//     // many
//     for (int i = 0; i < 12; i++) {
//         entry_t* test_entry3 = init_entry("Starbucks", test_date1, -0.23, 2,
//             "Zionist scum!");
//         entry_node_t* test_entry_node3 = init_entry_node(test_entry3);
//         append_to_tail(el, test_entry_node3);
//     }
// 
//     b = init_browser(el, -1);
//     do {
// 	draw_browser(b, ca);
// 	ch = wgetch(b->win);
// 	result = browser_handle_key(b, ch);
//     } while(result);
// 
//     free_browser(b);
// 
//     free_cat_array(ca);
//     free_entry_list(el);
//     free_prompt(p);

    end_budgurse();
}

void init_budgurse() {

    // start ncurses
    initscr();
    curs_set(0);
    use_default_colors();

    init_wins();

    entries = init_entry_list();
    categories = init_cat_array();

    db = NULL;
    int rc = init_db("data/budgurse.db");
    if(rc) {
	printf("AH\n");
	exit(1);
    }

    rc = load_db(db, entries, categories);
    if(rc) {
	printf("AH\n");
	exit(1);
    }

    browser = init_browser(-1);

    state = BROWSER;
}


int handle_input(int ch) {
    switch (state) {
	case BROWSER: return browser_handle_key(ch);
    }
    return 1;
}


void end_budgurse() {
    
    free_browser(browser);
    free_cat_array(categories);
    free_entry_list(entries);
    free_wins();

    int result = sqlite3_close(db);
    if(result)
	exit(1);
    
    // stop ncurses
    endwin();
}
