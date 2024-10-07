#include "budgurse.h"

state_t state;

int main(int argc, char *argv[]) {

    init_budgurse();
    draw_browser();

    while(1) {
	int ch = wgetch(g_wins[state].win);
	if (!handle_input(ch))
	    break;
    }

    end_budgurse(EXIT_SUCCESS);
    return 0;
}


void init_budgurse() {

    // start ncurses
    initscr();
    curs_set(0);
    noecho();
    use_default_colors();

    // init global array g_wins
    init_wins();
    g_entries = init_entry_list();
    g_categories = init_cat_array();

    init_db("data/budgurse.db");
    load_db();

    g_browser = init_browser(g_entries, -1);

    state = BROWSER;
}


int handle_input(int ch) {
    switch (state) {
	case BROWSER: return browser_handle_key(ch);
	case PROMPT: return 1;
    }
    return 1;
}


void end_budgurse(int status) {
    
    free_browser(g_browser);
    free_cat_array(g_categories);
    free_entry_list(g_entries);
    free_wins(g_wins);
    if (sqlite3_close(g_db))
	ERROR_MSG("Failed to properly close database with error message: %s\n",
		  sqlite3_errmsg(g_db));

    // stop ncurses
    endwin();
    exit(status);
}
