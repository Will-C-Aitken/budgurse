#include "help.h"

help_t *g_help = NULL;

const char *help_table_b[BROWSER_KEYS*2] = {
    "a", "Add entry",
    "d", "Delete selected entry",
    "e", "Edit selected entry",
    "s", "Switch to summary window",
    "j, down", "Scroll down",
    "h, up", "Scroll up",
    "gg", "Scroll to earliest/top entry",
    "G", "Scroll to latest/bottom entry",
    "q", "Quit program"
};

const char *help_table_s[SUMMARY_KEYS*2] = {
    "d", "Delete selected category",
    "e", "Edit selected category's name",
    "b", "Switch to browser window",
    "m", "Move context one month forward",
    "M", "Move context one month backward",
    "y", "Move context one year forward",
    "Y", "Move context one year backward",
    "h, left", "Scroll left",
    "l, right", "Scroll right",
    "j, down", "Scroll down",
    "h, up", "Scroll up",
    "0", "Scroll to earliest visible date",
    "$", "Scroll to right end/category totals",
    "gg", "Scroll to first category",
    "G", "Scroll to last/total category",
    "q", "Quit program"
};


help_t *init_help(state_t s) {
    help_t *h = malloc(sizeof(help_t));

    switch (s) {
        case BROWSER:
            h->txt_table = help_table_b;
	    h->num_rows = BROWSER_KEYS;
            break;
        case SUMMARY:
            h->txt_table = help_table_s;
	    h->num_rows = SUMMARY_KEYS;
            break;
        case PROMPT:
	    EXIT("Cannot open help from the prompt window");
        case HELP:
	    EXIT("Cannot open help from the help window");
    }

    h->first_row_idx = 0;
    h->prior_state = s;

    return h;
}


void free_help(help_t *h) {
    if (h)
	free(h);
}


void help_draw() {

    const char **t_table = g_help->txt_table;
    werase(g_wins[HELP].win);
    box(g_wins[HELP].win, 0, 0);
    mvwaddstr(g_wins[HELP].win, 0, 2, "budgurse");

    int row = 2;
    for (int i = g_help->first_row_idx; i < g_help->num_rows; i++) {
	if (row >= HELP_HEIGHT - 3) {
	    wmove(g_wins[HELP].win, row, (HELP_WIDTH/2) - 4);
	    draw_str(g_wins[HELP].win, "- more -", 8, "", 0);
	    break;
	}
	wmove(g_wins[HELP].win, row, 3);
	draw_ra_string(g_wins[HELP].win, t_table[i*2], 8, "", 0);
	draw_str(g_wins[HELP].win, t_table[(i*2)+1], HELP_WIDTH-13, " ", 0);
	row++;
    }

    wmove(g_wins[HELP].win, ++row, HELP_WIDTH-18);
    draw_str(g_wins[HELP].win, "press q to close", 16, "", 0);

    wrefresh(g_wins[HELP].win);
}


void help_scroll(dir_t d) {
    switch (d) {
	case UP:
	    if (g_help->first_row_idx > 0)
		g_help->first_row_idx--;
	    break;
	case DOWN:
	    if (g_help->first_row_idx + HELP_HEIGHT - 5 < g_help->num_rows)
		g_help->first_row_idx++;
	    break;
	case LEFT: 
	    EXIT("Cannot scroll help left");
	    break;
	case RIGHT:
	    EXIT("Cannot scroll help right");
	    break;
    }
}


int help_handle_key(int ch) {

    switch (ch) {
	case 'j':
	case KEY_DOWN:
	    help_scroll(DOWN);
	    break;
	case 'k':
	case KEY_UP:
	    help_scroll(UP);
	    break;
	case 'q':
	    state = g_help->prior_state;
	    free_help(g_help);
	    g_help = NULL;
	    break;
    }

    return 1;
}
