#include "browser.h"

browser_t *g_browser = NULL;

// set max_num_entries to -1 to calculate it with actual window size
browser_t* init_browser(entry_list_t* el, int max_num_entries){

    browser_t *b = malloc(sizeof(browser_t));

    if (max_num_entries == -1)
	b->max_num_entries = g_wins[BROWSER].h - 4;
    else
	b->max_num_entries = max_num_entries;

    if (!el || (!el->num_nodes) || (b->max_num_entries < 1)) {
	b->start = b->sel = b->end = NULL;
	b->num_entries = 0;
	return b;
    }

    // select tail at startup
    b->end = b->sel = b->start = el->tail;
    b->num_entries = 1;
    while (b->num_entries < b->max_num_entries && !is_head(b->start)) {
	b->start = b->start->prev;
	b->num_entries++;
    }

    return b;
}

int browser_handle_key(int ch) {
    switch (ch) {
	case 'j':
	case KEY_DOWN:
	    browser_scroll(1, DOWN);
	    break;
	case 'k':
	case KEY_UP:
	    browser_scroll(1, UP);
	    break;
	case 'q':
	    return 0;
    }
    return 1;
}

void browser_scroll(int num_times, direction_t dir) {

    EXIT_IF(!g_browser, "Browser not initialized");

    while (num_times > 0) {

	if (g_browser->num_entries < 1)
	    return;

	// break if at either end of list
 	if ((is_head(g_browser->sel) && dir == UP) || 
	    (is_tail(g_browser->sel) && dir == DOWN))
 	    return;

	wprintw(g_wins[PROMPT].win, "Here");
	wrefresh(g_wins[PROMPT].win);


	// move start and end points if at end of current context 
	if ((g_browser->sel == g_browser->start && dir == UP) ||
	    (g_browser->sel == g_browser->end && dir == DOWN)) {
	    entry_node_traverse(&g_browser->end, dir);
	    entry_node_traverse(&g_browser->start, dir);
	}

	entry_node_traverse(&g_browser->sel, dir);

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
    if (!b)
	return;
    free(b);
    b = NULL;
}


void draw_browser() {

    box(g_wins[BROWSER].win, 0, 0);

    browser_draw_header();

    // draw hline after column headers
    int row = 2;
    wmove(g_wins[BROWSER].win, row, 0);
    waddch(g_wins[BROWSER].win, ACS_LTEE);
    whline(g_wins[BROWSER].win, ACS_HLINE, g_wins[BROWSER].w - 2);
    wmove(g_wins[BROWSER].win, row++, g_wins[BROWSER].w - 1);
    waddch(g_wins[BROWSER].win, ACS_RTEE);

    if (!g_browser->start) {
	wrefresh(g_wins[BROWSER].win);
	return;
    }

    entry_node_t* temp = g_browser->start;
    while (temp != g_browser->end->next) {
	if (temp == g_browser->sel)
	    wattron(g_wins[BROWSER].win, A_REVERSE);
        browser_draw_entry(temp->data, row++);
	if (temp == g_browser->sel)
	    wattroff(g_wins[BROWSER].win, A_REVERSE);
	temp = temp->next;
    } 

    wrefresh(g_wins[BROWSER].win);
}


void browser_draw_header() {
    int col = 2;
    char *col_names[] = {"Date", "Name", "     Amount", "Category", 
		       "Subcategory"};

    for (int i = 0; i < 5; i++) {
	wmove(g_wins[BROWSER].win, 1, col);
	browser_draw_string(col_names[i], &col, browser_col_widths[i]);
    }
}


void browser_draw_entry(const entry_t *e, int row) {

    int col = 1;
    int i = 0;

    wmove(g_wins[BROWSER].win, row, col);
    waddch(g_wins[BROWSER].win, ' ');
    col++;
    wmove(g_wins[BROWSER].win, row, col);

    browser_draw_date(e->date, &col, browser_col_widths[i++]);
    wmove(g_wins[BROWSER].win, row, col);

    browser_draw_string(e->name, &col, browser_col_widths[i++]);
    wmove(g_wins[BROWSER].win, row, col);

    browser_draw_amount(e->amount, &col, browser_col_widths[i++]);
    wmove(g_wins[BROWSER].win, row, col);

    char* cat;
    char* subcat;
    cat_id_to_names(g_categories, e->category_id, &cat, &subcat);

    browser_draw_string(cat, &col, browser_col_widths[i++]);
    wmove(g_wins[BROWSER].win, row, col);

    if (subcat)
	browser_draw_string(subcat, &col, browser_col_widths[i++]);
    else
	browser_draw_string("", &col, browser_col_widths[i++]);

    // pad with blank spaces
    wmove(g_wins[BROWSER].win, row, col);
    browser_draw_string("", &col, getmaxx(g_wins[BROWSER].win) - col - 3);
}


void browser_draw_date(time_t date, int *col, int max_width){

    struct tm *tmp_date = gmtime(&date);
    wprintw(g_wins[BROWSER].win, "%02d/%02d/%04d", tmp_date->tm_mon, 
	    tmp_date->tm_mday, tmp_date->tm_year + 1900);
    *col += max_width;
}


void browser_draw_string(const char *str, int *col, int max_width){

    // only draw "   " if not first col
    if (*col > 2) {
	waddstr(g_wins[BROWSER].win, "   ");
    }

    // trunacte string
    char trunc_str[max_width];
    snprintf(trunc_str, max_width, "%-*s", max_width, str);
    wprintw(g_wins[BROWSER].win, "%s", trunc_str);
    *col += max_width;
}


void browser_draw_amount(float amount, int *col, int max_width){

    waddstr(g_wins[BROWSER].win, "   ");

    // to right align
    int num_spaces, num_dig, thousands;
    num_dig = num_places_in_amount((int)amount);
    num_spaces = max_width - 8 - num_dig;
    thousands = abs((int)amount / 1000);

    // for comma
    if (thousands) 
	num_spaces--;
    while (num_spaces-- > 0) 
	waddch(g_wins[BROWSER].win, ' ');

    // print negative (if necessary) and dollar sign 
    if (amount < 0) 
	waddstr(g_wins[BROWSER].win, "-$");
    else 
	waddstr(g_wins[BROWSER].win, " $");
    amount = fabs(amount);

    // print comma in thousands
    if (thousands) 
	wprintw(g_wins[BROWSER].win, "%d,%0.2f", thousands, 
		amount - (thousands*1000));
    else
	wprintw(g_wins[BROWSER].win, "%0.2f", amount);

    *col += max_width; 
}


int num_places_in_amount(int n) {
    if (n < 0) return num_places_in_amount((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + num_places_in_amount(n / 10);
}
