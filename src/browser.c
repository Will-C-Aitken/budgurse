#include "browser.h"


void draw_browser(WINDOW *browser_win, const entry_list_t *el,
	entry_t *sel_entry) {

    clear();
    box(browser_win, 0, 0);

    // -2 for border space
    int max_row, max_col;
    getmaxyx(browser_win, max_row, max_col);

    browser_draw_header(browser_win);

    // draw hline after column headers
    int row = 2;
    wmove(browser_win, row, 0);
    waddch(browser_win, ACS_LTEE);
    whline(browser_win, ACS_HLINE, max_col + 1);
    wmove(browser_win, row++, max_col - 1);
    waddch(browser_win, ACS_RTEE);

    // Determine how many entries to print (excluding borders and header)
    int count = (el->num_nodes < (max_row - 4)) ? el->num_nodes : max_row - 4;
    row = count + 2;

    entry_t *temp = sel_entry;
    while(count) {
	browser_draw_entry(browser_win, temp, row--);
	temp = temp->prev;
	count--;
    }
}


void browser_draw_header(WINDOW *browser_win) {
    int col = 2;
    char *col_names[] = {"Date", "Name", "Amount", "Category", 
		       "Subcategory"};

    for (int i = 0; i < 5; i++) {
	wmove(browser_win, 1, col);
	browser_draw_string(browser_win, col_names[i], &col, 
		browser_col_widths[i]);
    }
}


void browser_draw_entry(WINDOW *browser_win, const entry_t *e, int row) {

    int col = 2;
    int i = 0;
    wmove(browser_win, row, col);

    browser_draw_date(browser_win, e->date, &col, browser_col_widths[i++]);
    wmove(browser_win, row, col);

    browser_draw_string(browser_win, e->name, &col, browser_col_widths[i++]);
    wmove(browser_win, row, col);

    browser_draw_amount(browser_win, e->amount, &col, browser_col_widths[i++]);
    wmove(browser_win, row, col);

    browser_draw_string(browser_win, e->category, &col, 
	    browser_col_widths[i++]);
    wmove(browser_win, row, col);

    if (e->subcategory)
	browser_draw_string(browser_win, e->subcategory, &col, 
		browser_col_widths[i++]);
    else
	browser_draw_string(browser_win, "", &col, browser_col_widths[i++]);
    wmove(browser_win, row, col);
}


void browser_draw_date(WINDOW *browser_win, time_t date, int *col, 
	int max_width){

    struct tm *tmp_date = gmtime(&date);
    wprintw(browser_win, "%02d/%02d/%04d", tmp_date->tm_mon, 
	    tmp_date->tm_mday, tmp_date->tm_year + 1900);
    *col += max_width;
}


void browser_draw_string(WINDOW* browser_win, const char *str, int *col, 
	int max_width){

    // only draw " | " if not first col
    if (*col > 2) {
	waddch(browser_win, ' ');
	waddch(browser_win, ACS_VLINE);
	waddch(browser_win, ' ');
    }

    // trunacte string
    char trunc_str[max_width];
    snprintf(trunc_str, max_width, "%s", str);
    wprintw(browser_win, "%s", trunc_str);
    *col += max_width;
}


void browser_draw_amount(WINDOW *browser_win, float amount, int *col,
	int max_width){

    waddch(browser_win, ' ');
    waddch(browser_win, ACS_VLINE);
    waddch(browser_win, ' ');

    // to right align
    int num_spaces, num_dig, thousands;
    num_dig = num_places_in_amount((int)amount);
    num_spaces = max_width - 8 - num_dig;
    thousands = abs((int)amount / 1000);

    // for comma
    if (thousands) 
	num_spaces--;
    while (num_spaces-- > 0) 
	waddch(browser_win, ' ');

    // print negative (if necessary) and dollar sign 
    if (amount < 0) 
	waddstr(browser_win, "-$");
    else 
	waddstr(browser_win, " $");
    amount = fabs(amount);

    // print comma in thousands
    if (thousands) 
	wprintw(browser_win, "%d,%0.2f", thousands, amount - (thousands*1000));
    else
	wprintw(browser_win, "%0.2f", amount);

    *col += max_width; 
}


int num_places_in_amount (int n) {
    if (n < 0) return num_places_in_amount((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + num_places_in_amount(n / 10);
}
