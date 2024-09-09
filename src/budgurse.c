#include "budgurse.h"

int main(int argc, char *argv[]) {

    sqlite3 *db = NULL;
    int result = init_db(&db, "data/budgurse.db");

    if(result)
	exit(1);

    entry_list_t *entries = init_entry_list();
    result = load_db(db, entries);
    
    if(result)
	exit(1);

    // start ncurses
    initscr();
    curs_set(0);

    int max_row = getmaxy(stdscr);
    browser_buf_t* browser_entries = init_browser_buf(entries, max_row);
    draw_browser(stdscr, browser_entries);

    getch();
    endwin();

    free_entry_list(entries);
    result = sqlite3_close(db);

    if(result)
	exit(1);

    return 0;
}

