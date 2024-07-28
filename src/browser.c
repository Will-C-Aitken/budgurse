#include "browser.h"


void draw_browser(WINDOW *browser_win, const entry_list_t *el) {

    // -2 for border a space
    int max_col = getmaxx(browser_win);

    box(browser_win, 0, 0);
    browser_draw_header(browser_win, max_col);

    // draw hline after column headers
    int row = 2;
    wmove(browser_win, row, 0);
    waddch(browser_win, ACS_LTEE);
    whline(browser_win, ACS_HLINE, max_col + 1);
    wmove(browser_win, row++, max_col - 1);
    waddch(browser_win, ACS_RTEE);

    // draw entry list
    entry_node_t *temp = el->head;
    while(temp) {
	browser_draw_entry(browser_win, temp->data, row++, max_col);
	temp = temp->next;
    }
}


void browser_draw_header(WINDOW *browser_win, int max_col) {
    int col = 2;
    int col_lens[] = {10, 20, 10, 15, 15, 15};
    char *col_names[] = {"Date", "Name", "Amount", "Category", 
		       "Subcategory", "Notes"};

    for (int i = 0; i < 6; i++) {
	wmove(browser_win, 1, col);
	browser_draw_string(browser_win, col_names[i], &col, col_lens[i]);
    }
}


void browser_draw_entry(WINDOW *browser_win, const entry_t *e, int row, 
	int max_col){

    int col = 2;
    wmove(browser_win, row, col);

    browser_draw_date(browser_win, e->date, &col);
    wmove(browser_win, row, col);

    browser_draw_string(browser_win, e->name, &col, 20);
    wmove(browser_win, row, col);

    browser_draw_amount(browser_win, e->amount, &col);
    wmove(browser_win, row, col);

    browser_draw_string(browser_win, e->category, &col, 15);
    wmove(browser_win, row, col);

    if (e->subcategory)
	browser_draw_string(browser_win, e->subcategory, &col, 15);
    else
	browser_draw_string(browser_win, "", &col, 15);
    wmove(browser_win, row, col);

    if (e->note)
	browser_draw_string(browser_win, e->note, &col, 15);
    else
	browser_draw_string(browser_win, "", &col, 15);
    wmove(browser_win, row, col);
}


void browser_draw_date(WINDOW *browser_win, time_t date, int *col){
    struct tm *tmp_date = gmtime(&date);
    wprintw(browser_win, "%02d/%02d/%04d", tmp_date->tm_mon, 
	    tmp_date->tm_mday, tmp_date->tm_year + 1900);
    *col += 10;
}


void browser_draw_string(WINDOW* browser_win, const char *str, int *col, 
	int max_len){

    if (*col > 2) {
	waddch(browser_win, ' ');
	waddch(browser_win, ACS_VLINE);
	waddch(browser_win, ' ');
    }

    wprintw(browser_win, "%s", str);
    *col += max_len;
}


void browser_draw_amount(WINDOW *browser_win, float amount, int *col){
    waddch(browser_win, ' ');
    waddch(browser_win, ACS_VLINE);
    waddch(browser_win, ' ');

    int num_left_space = 10;
    if (amount < 0)
	wprintw(browser_win, "-$%'.2f", -amount);
    else
	wprintw(browser_win, " $%'.2f", amount);

    *col += 10; 
}
