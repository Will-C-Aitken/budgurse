/* budgurse - budgeting with curses
 *
 * Copyright (c) 2025 W. C. Aitken 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "budgurse.h"

int curses_mode = 1;
state_t state = BROWSER;

int main(int argc, char *argv[]) {

    budgurse_t *b = NULL;
    init_budgurse(&b);

    while(1) {
	draw(state);
	int ch = wgetch(g_wins[state].win);
	if (ch == KEY_RESIZE) {
	    if (!resize())
		break;
	    else
		continue;
	}
	if (!handle_input(b, ch))
	    break;
    }

    end_budgurse(EXIT_SUCCESS);
    return 0;
}


void init_budgurse(budgurse_t **b) {

    char *db_path = NULL;
    *b = malloc(sizeof(budgurse_t));

    // start ncurses
    initscr();
    curs_set(0);
    keypad(stdscr, true);
    noecho();
    use_default_colors();

    // init global array g_wins
    init_wins();
    g_categories = init_llist();

    init_data_path(&db_path);
    init_db(&(b->db), db_path);
    free(db_path);

    g_categories = init_llist();
    load_cat_table(b->db);

    date_context_t *dc = init_date_context(0, 0, MONTH);
    g_entry_list = init_entry_list(b, dc);

    g_browser = init_browser(g_entry_list->entries, 
	    g_entry_list->entries->tail, 0, -1);
    g_summary = init_summary(0, MONTH, -1, -1, -1, -1);
    summary_calc();

    resize();

    state = BROWSER;
}


int handle_input(budgurse_t *b, int ch) {
    switch (state) {
	case BROWSER: return browser_handle_key(b, ch);
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

int resize() {
    if (!wins_resize())
	return 0;
    browser_resize();
    summary_resize();
    return 1;
}

void end_budgurse(budgurse_t *b, int status) {
    
    free_browser(g_browser);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    free_entry_list(g_entry_list);
    free_wins(g_wins);
    free_summary(g_summary);

    if (g_help)
 	free_help(g_help);

    if (sqlite3_close(b->db))
	ERROR_MSG("Failed to properly close database with error message: %s\n",
	    sqlite3_errmsg(b->db));
    free(b);

    // stop ncurses
    endwin();
    exit(status);
}
