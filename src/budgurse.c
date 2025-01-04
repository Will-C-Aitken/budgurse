#include "budgurse.h"

int curses_mode = 1;
state_t state = BROWSER;

int main(int argc, char *argv[]) {

    init_budgurse();

    while(1) {
	draw(state);
	int ch = wgetch(g_wins[state].win);
	if (!handle_input(ch))
	    break;
    }

    end_budgurse(EXIT_SUCCESS);
    return 0;
}


void init_budgurse() {

    char *db_path = NULL;

    // start ncurses
    initscr();
    curs_set(0);
    keypad(stdscr, true);
    noecho();
    use_default_colors();

    // init global array g_wins
    init_wins();
    g_entries = init_llist();
    g_categories = init_llist();

    init_data_path(&db_path);
    init_db(db_path);
    load_db();
    free(db_path);

    g_browser = init_browser(g_entries, g_entries->tail, 0, -1);
    g_summary = init_summary(0, MONTH, -1, -1);
    summary_calc();

    state = BROWSER;
}


int handle_input(int ch) {
    switch (state) {
	case BROWSER: return browser_handle_key(ch);
	case SUMMARY: return summary_handle_key(ch);
	case HELP: return help_handle_key(ch);
	// prompt has no need to be selectable for now
	case PROMPT: return 1;
    }
    return 1;
}


void draw(state_t s) {
    switch (state) {
	case BROWSER: browser_draw(); break;
	case SUMMARY: summary_draw(); break;
	case HELP: help_draw(); break;
	// prompt is drawn while receiving input
	case PROMPT: return;
    }
}


void end_budgurse(int status) {
    
    free_browser(g_browser);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_wins(g_wins);
    free_summary(g_summary);

    if (g_help)
 	free_help(g_help);

    if (sqlite3_close(g_db))
	ERROR_MSG("Failed to properly close database with error message: %s\n",
		  sqlite3_errmsg(g_db));

    // stop ncurses
    endwin();
    exit(status);
}
