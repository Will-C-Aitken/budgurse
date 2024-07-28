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
    draw_browser(stdscr, entries);
    getch();
    endwin();

    result = sqlite3_close(db);

    if(result)
	exit(1);

    return 0;
}

