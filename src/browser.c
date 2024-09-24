#include "browser.h"

int browser_handle_key(browser_t* b, int ch) {
    switch (ch) {
    case 'j':
    case KEY_DOWN:
	browser_scroll(b, 1, DOWN);
	break;
    case 'k':
    case KEY_UP:
	browser_scroll(b, 1, UP);
	break;
    case 'q':
	return 0;
    }
    return 1;
}


browser_t* init_browser(const entry_list_t *el, int num_browser_rows){

    browser_t *b = malloc(sizeof(browser_t));
    b->max_num_entries = num_browser_rows - 4;

    if (!el || (!el->num_nodes) || (b->max_num_entries < 1)) {
	b->start = b->sel = b->end = NULL;
	b->num_entries = 0;
	return b;
    }

    // select tail at startup
    b->end = b->sel = b->start = el->tail;
    b->num_entries = 1;
    while (num_browser_rows > 5 && !is_head(b->start)) {
	b->start = b->start->prev;
	num_browser_rows--;
	b->num_entries++;
    }

    return b;
}


void browser_scroll(browser_t* b, int num_times, direction_t dir) {
    while (num_times > 0) {

	// break if at either end of list
 	if ((is_head(b->sel) && dir == UP) || (is_tail(b->sel) && dir == DOWN))
 	    return;

	// move start and end points if at end of current context 
	if ((b->sel == b->start && dir == UP) ||
	    (b->sel == b->end && dir == DOWN)) {
	    entry_node_traverse(&b->end, dir);
	    entry_node_traverse(&b->start, dir);
	}

	entry_node_traverse(&b->sel, dir);

	num_times--;
    }
}

// int browser_go_to(browser_t* b, entry_t *dest, direction_t dir) {
//     
//     while (b->sel_node->data != dest) {
// 	// we've reached either end of the list and should go no further
// 	if (!entry_node_traverse(b->sel_node, dir))
// 	    return 1;
// 
// 	b->sel_node = entry_node_traverse(b->sel_node, dir);
// 	if (entry_node_traverse(b->start_node, dir))
// 	    b->start_node = entry_node_traverse(b->start_node, dir);
// 	b->end_node = entry_node_traverse(b->prev_node, dir);
//     }
// 
//     return 0;
// }


void free_browser(browser_t* b) {
    free(b);
}


void draw_browser(WINDOW *browser_win, const browser_t *b) {

    //clear();
    box(browser_win, 0, 0);

    // -2 for border space
    int max_col;
    max_col = getmaxx(browser_win);

    browser_draw_header(browser_win);

    // draw hline after column headers
    int row = 2;
    wmove(browser_win, row, 0);
    waddch(browser_win, ACS_LTEE);
    whline(browser_win, ACS_HLINE, max_col + 1);
    wmove(browser_win, row++, max_col - 1);
    waddch(browser_win, ACS_RTEE);

    if (!b->start)
	return;

    entry_node_t* temp = b->start;
    while (temp != b->end->next) {
	if (temp == b->sel)
	    attron(A_REVERSE);
        browser_draw_entry(browser_win, temp->data, row++);
	if (temp == b->sel)
	    attroff(A_REVERSE);
	temp = temp->next;
    } 
}


void browser_draw_header(WINDOW *browser_win) {
    int col = 2;
    char *col_names[] = {"Date", "Name", "     Amount", "Category", 
		       "Subcategory"};

    for (int i = 0; i < 5; i++) {
	wmove(browser_win, 1, col);
	browser_draw_string(browser_win, col_names[i], &col, 
		browser_col_widths[i]);
    }
}


void browser_draw_entry(WINDOW *browser_win, const entry_t *e, int row) {

    int col = 1;
    int i = 0;
    wmove(browser_win, row, col);
    addch(' ');
    col++;
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

    // pad with blank spaces
    wmove(browser_win, row, col);
    browser_draw_string(browser_win, "", &col, getmaxx(browser_win) - col - 3);
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

    // only draw "   " if not first col
    if (*col > 2) {
	waddstr(browser_win, "   ");
    }

    // trunacte string
    char trunc_str[max_width];
    snprintf(trunc_str, max_width, "%-*s", max_width, str);
    wprintw(browser_win, "%s", trunc_str);
    *col += max_width;
}


void browser_draw_amount(WINDOW *browser_win, float amount, int *col,
	int max_width){

    waddstr(browser_win, "   ");

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


int num_places_in_amount(int n) {
    if (n < 0) return num_places_in_amount((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + num_places_in_amount(n / 10);
}


int browser_max_entries(int num_nodes_in_list, int num_rows_in_window) {
    // minus four for header and three border lines
    int num_rows = num_rows_in_window - 4;
    return (num_nodes_in_list < num_rows) ? num_nodes_in_list : num_rows;
}
