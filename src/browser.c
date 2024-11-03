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

    EXIT_IF(!g_browser, "Browser not initialized");

    switch (ch) {
	case 'a':
	    browser_add_entry();
	    break;
	case 'd':
	    browser_del_entry();
	    break;
	case 'e':
	    browser_edit_entry();
	    break;
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
    draw_browser();
    return 1;
}


void browser_scroll(int num_times, direction_t dir) {

    if (g_browser->num_entries < 1)
	return;

    while (num_times > 0) {

	if (g_browser->num_entries < 1)
	    return;

	// break if at either end of list
 	if ((is_head(g_browser->sel) && dir == UP) || 
	    (is_tail(g_browser->sel) && dir == DOWN))
 	    return;

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


void browser_add_entry() {
    entry_node_t *new_en = prompt_new_entry();
    // user exited
    if (!new_en)
	return;
    browser_insert(new_en);
 }


// Here is why an array for entries may have been a better choice. The logic
// would certainly be simpler. However, since most new entries will be at the
// tail and close to the current context the linked list should be faster.
void browser_insert(entry_node_t *en) {

    // browser is already full
    if (g_browser->num_entries == g_browser->max_num_entries) {
	// check if new entry has expanded current context (distance will equal
	// to num_entries when normally it's num_entries - 1)
	int dist = dist_between(g_browser->start, g_browser->end);
	if (dist == g_browser->num_entries) {
	    // if yes, make room and return
	    entry_node_traverse(&g_browser->start, DOWN);
	    g_browser->sel = en;
	    return;
	}

	// otherwise, try going down first 
	while (g_browser->sel->next) {
	    entry_node_traverse(&g_browser->sel, DOWN);
	    // only move context down until you no longer can
	    if (g_browser->end->next) {
		entry_node_traverse(&g_browser->start, DOWN);
		entry_node_traverse(&g_browser->end, DOWN);
	    }
	    if (g_browser->sel == en)
		return;
	}

	// if still not found, go back up
	// worst case go all the way down then all the way back up, but very
	// rare (would only happen if near the top, and add something just
	// above it, outside of context)
	g_browser->sel = g_browser->start;
	while (g_browser->sel->prev) {
	    entry_node_traverse(&g_browser->sel, UP);
	    // only move context up until you no longer can
	    if (g_browser->start->prev) {
		entry_node_traverse(&g_browser->start, UP);
		entry_node_traverse(&g_browser->end, UP);
	    }
	    if (g_browser->sel == en)
		return;
	}
    }

    // browser is not full yet
    else {
	g_browser->num_entries++;

	g_browser->sel = en;
	g_browser->start = g_entries->head;
	g_browser->end = g_entries->tail;
	return;
    }
}


void browser_edit_entry() {
    if (g_browser->num_entries > 0) 
	prompt_edit_entry(g_browser->sel);
}


void browser_del_entry() {
    entry_node_t *en_to_del = browser_pop_sel_entry();
    if (en_to_del) {
	db_exec(en_to_del->data, (gen_sql_fn_t)del_entry_to_sql);
	del_entry(g_entries, en_to_del);
    }
}



entry_node_t *browser_pop_sel_entry() {

    // if already empty, cannot delete
    if (g_browser->num_entries == 0)
	return NULL;

    entry_node_t *temp = g_browser->sel;

    // check if removed entry will be replaced by another in browser (i.e.
    // there's more nodes that aren't visible). In this case we want to move UP
    if (g_entries->num_nodes > g_browser->num_entries) {
	// start needs to go up to show next node
	entry_node_traverse(&g_browser->start, UP);

	// tail only moves up if it's being deleted
	if (g_browser->sel == g_browser->end) 
	    entry_node_traverse(&g_browser->end, UP);
	
	entry_node_traverse(&g_browser->sel, UP);
    }

    // otherwise all nodes are visible and deleting should move sel DOWN 
    else {
	// in this case deleting an entry decreases number in browser as well
	g_browser->num_entries--;

	// only touch start if it's being deleted
	if (g_browser->sel == g_browser->start)
	    entry_node_traverse(&g_browser->start, DOWN);
	
	// tail (and sel now) only moves up if it's being deleted 
	if (g_browser->sel == g_browser->end) {
	    entry_node_traverse(&g_browser->end, UP);
	    entry_node_traverse(&g_browser->sel, UP);
	}
	else
	    entry_node_traverse(&g_browser->sel, DOWN);
    }
    
    if (g_browser->num_entries == 0)
	g_browser->sel = g_browser->end = g_browser->start = NULL;

    return temp;
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

    werase(g_wins[BROWSER].win);
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
    char *col_names[] = {"Date", "Name", "     Amount", "Category", 
		       "Subcategory"};

    wmove(g_wins[BROWSER].win, 1, 1);
    for (int i = 0; i < 5; i++)
	if (i == 0)
	    browser_draw_string(col_names[i], browser_col_widths[i], " ");
	else
	    browser_draw_string(col_names[i], browser_col_widths[i], "  ");
}


void browser_draw_entry(const entry_t *e, int row) {
    const char *delim_str = "  ";
    int i = 0;

    // skip border
    wmove(g_wins[BROWSER].win, row, 1);
    waddch(g_wins[BROWSER].win, ' ');

    browser_draw_date(e->date, browser_col_widths[i++]);
    browser_draw_string(e->name, browser_col_widths[i++], delim_str);
    browser_draw_amount(e->amount, browser_col_widths[i++], delim_str);

    char* cat;
    char* subcat;
    cat_id_to_names(g_categories, e->category_id, &cat, &subcat);
    browser_draw_string(cat, browser_col_widths[i++], delim_str);
    if (subcat)
	browser_draw_string(subcat, browser_col_widths[i++], delim_str);
    else
	browser_draw_string("", browser_col_widths[i++], delim_str);

    // pad with blank spaces
    int pad_len = getmaxx(g_wins[BROWSER].win) - getcurx(g_wins[BROWSER].win) - 1;
    browser_draw_string("", pad_len, "");
}


void browser_draw_date(time_t date, int max_width){
    struct tm *tmp_date = gmtime(&date);
    wprintw(g_wins[BROWSER].win, "%02d/%02d/%04d", ++(tmp_date->tm_mon), 
	    tmp_date->tm_mday, tmp_date->tm_year + 1900);
}


void browser_draw_string(const char *str, int max_width, 
	const char *delim_str){

    waddstr(g_wins[BROWSER].win, delim_str);

    // trunacte string
    char trunc_str[max_width+1];
    snprintf(trunc_str, max_width+1, "%-*s", max_width, str);
    wprintw(g_wins[BROWSER].win, "%s", trunc_str);
}


void browser_draw_amount(float amount, int max_width, const char *delim_str){
    waddstr(g_wins[BROWSER].win, delim_str);

    // to right align
    int num_spaces, num_dig, thousands;
    num_dig = num_places_in_amount((int)amount);
    num_spaces = max_width - 5 - num_dig;
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
	wprintw(g_wins[BROWSER].win, "%d,%06.2f", thousands, 
		amount - (thousands*1000));
    else
	wprintw(g_wins[BROWSER].win, "%0.2f", amount);
}


int num_places_in_amount(int n) {
    if (n < 0) return num_places_in_amount((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + num_places_in_amount(n / 10);
}
