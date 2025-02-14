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

#include "browser.h" 

browser_t *g_browser = NULL;

// set max_num_entries to -1 to calculate it with actual window size
// set sel_to_end to -1 to assume halfway (sel_to_end is the distance between
// the selected node and the end node)
browser_t* init_browser(llist_t *el, llist_node_t *sel_node, int sel_to_end, 
	int max_num_entries) {

    browser_t *b = malloc(sizeof(browser_t));

    if (max_num_entries == -1)
	b->max_num_entries = g_wins[BROWSER].h - 4;
    else
	b->max_num_entries = max_num_entries;

    if (!el || !sel_node || (!el->num_nodes) || (b->max_num_entries < 1)) {
	b->start = b->sel = b->end = NULL;
	b->num_entries = 0;
	return b;
    }

    b->end = b->sel = b->start = sel_node;
    b->num_entries = 1;

    if (sel_to_end == -1)
	sel_to_end = (b->max_num_entries - 1)/2;

    while (b->num_entries <= sel_to_end && !llist_is_tail(b->end)) {
	b->end = b->end->next;
	b->num_entries++;
    }

    while ((b->num_entries < b->max_num_entries)
	    && !llist_is_head(b->start)) {
	b->start = b->start->prev;
	b->num_entries++;
    }

    // sometimes sel_to_end is the ideal case but is not possible (e.g. in the 
    // case of an edit that moves the entry near head or tail). In this case,
    // make sure as much of list is showing by extending the end even if it 
    // does not match sel_to_end
    while (b->num_entries < b->max_num_entries && !llist_is_tail(b->end)) {
	b->end = b->end->next;
	b->num_entries++;
    }

    return b;
}


void browser_resize() {
    llist_node_t *prior_sel = g_browser->sel;
    free_browser(g_browser);
    // just move sel to middle of browser
    g_browser = init_browser(g_entries, prior_sel, -1, -1);
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
	case 'g':
	    if (wgetch(g_wins[BROWSER].win) == 'g')
		browser_to_top();
	    break;
	case 'G':
	    free_browser(g_browser);
	    g_browser = init_browser(g_entries, g_entries->tail, 0, -1);
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
	case 's':
	    state = SUMMARY;
	    summary_calc();
	    break;
	case 'v':
	// enter key (carriage return)
	case 10:
	    browser_view_sel_entry();
	    break;
	case '?':
	    g_help = init_help(state);
	    state = HELP;
	    break;
    }

    return 1;
}


void browser_scroll(int num_times, dir_t dir) {

    if (g_browser->num_entries < 1)
	return;

    while (num_times > 0) {

	if (g_browser->num_entries < 1)
	    return;

	// break if at either end of list
 	if ((llist_is_head(g_browser->sel) && dir == UP) || 
	    (llist_is_tail(g_browser->sel) && dir == DOWN))
 	    return;

	// move start and end points if at end of current context 
	if ((g_browser->sel == g_browser->start && dir == UP) ||
	    (g_browser->sel == g_browser->end && dir == DOWN)) {
	    llist_node_traverse(&g_browser->end, dir);
	    llist_node_traverse(&g_browser->start, dir);
	}

	llist_node_traverse(&g_browser->sel, dir);

	num_times--;
    }
}


void browser_to_top() {
    if (g_browser->num_entries < 2 || g_browser->sel == g_entries->head)
	return;

    g_browser->start = g_browser->sel = g_browser->end = g_entries->head;
    for (int i = 0; i < g_browser->num_entries - 1; i++)
	g_browser->end = g_browser->end->next;
}


void browser_add_entry() {
    llist_node_t *new_en = prompt_new_entry_node();
    // user exited
    if (!new_en)
	return;
    browser_insert(new_en);
 }


// Here is why an array for entries may have been a better choice. The logic
// would certainly be simpler. However, since most new entries will be at the
// tail and close to the current context the linked list should be faster.
void browser_insert(llist_node_t *en) {

    // browser is already full
    if (g_browser->num_entries == g_browser->max_num_entries) {
	// check if new entry has expanded current context (distance will equal
	// to num_entries when normally it's num_entries - 1)
	int dist = llist_dist_between(g_browser->start, g_browser->end);
	if (dist == g_browser->num_entries) {
	    // if yes, make room and return
	    llist_node_traverse(&g_browser->start, DOWN);
	    g_browser->sel = en;
	    return;
	}

	// otherwise, try going down first 
	while (g_browser->sel->next) {
	    llist_node_traverse(&g_browser->sel, DOWN);
	    // only move context down until you no longer can
	    if (g_browser->end->next) {
		llist_node_traverse(&g_browser->start, DOWN);
		llist_node_traverse(&g_browser->end, DOWN);
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
	    llist_node_traverse(&g_browser->sel, UP);
	    // only move context up until you no longer can
	    if (g_browser->start->prev) {
		llist_node_traverse(&g_browser->start, UP);
		llist_node_traverse(&g_browser->end, UP);
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
    llist_node_t *en_to_del = browser_pop_sel_entry();
    if (en_to_del) {
	db_exec(en_to_del->data, (gen_sql_fn_t)del_entry_to_sql);
	llist_del_node(g_entries, en_to_del, (llist_free_data_fn_t)free_entry);
    }
}



llist_node_t *browser_pop_sel_entry() {

    // if already empty, cannot delete
    if (g_browser->num_entries == 0)
	return NULL;

    llist_node_t *temp = g_browser->sel;

    // check if removed entry will be replaced by another in browser (i.e.
    // there's more nodes that aren't visible). In this case we want to move UP
    if (g_entries->num_nodes > g_browser->num_entries) {
	// start needs to go up to show next node
	llist_node_traverse(&g_browser->start, UP);

	// tail only moves up if it's being deleted
	if (g_browser->sel == g_browser->end) 
	    llist_node_traverse(&g_browser->end, UP);
	
	llist_node_traverse(&g_browser->sel, UP);
    }

    // otherwise all nodes are visible and deleting should move sel DOWN 
    else {
	// in this case deleting an entry decreases number in browser as well
	g_browser->num_entries--;

	// only touch start if it's being deleted
	if (g_browser->sel == g_browser->start)
	    llist_node_traverse(&g_browser->start, DOWN);
	
	// tail (and sel now) only moves up if it's being deleted 
	if (g_browser->sel == g_browser->end) {
	    llist_node_traverse(&g_browser->end, UP);
	    llist_node_traverse(&g_browser->sel, UP);
	}
	else
	    llist_node_traverse(&g_browser->sel, DOWN);
    }
    
    if (g_browser->num_entries == 0)
	g_browser->sel = g_browser->end = g_browser->start = NULL;

    return temp;
}


void browser_view_sel_entry() {

    // nothing to view
    if (!g_browser || g_browser->num_entries == 0)
	return;

    entry_view(g_browser->sel->data);

}


void free_browser(browser_t* b) {
    if (!b)
	return;
    free(b);
}


void browser_draw() {

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

    llist_node_t* temp = g_browser->start;
    while (temp != g_browser->end->next) {
	if (temp == g_browser->sel)
	    wattron(g_wins[BROWSER].win, A_REVERSE);
        browser_draw_entry(temp->data, row++);
	if (temp == g_browser->sel)
	    wattroff(g_wins[BROWSER].win, A_REVERSE);
	temp = temp->next;
    } 

    prompt_default();
    wrefresh(g_wins[BROWSER].win);
}


void browser_draw_header() {
    char *col_names[] = {"Date", "Name", "     Amount", "Category", 
	"Subcategory"};

    wmove(g_wins[BROWSER].win, 1, 3);
    for (int i = 0; i < 5; i++) {
	if (i == 0)
	    draw_str(g_wins[BROWSER].win, col_names[i], browser_col_widths[i], 
		" ", 0);
	else
	    draw_str(g_wins[BROWSER].win, col_names[i], browser_col_widths[i], 
		"  ", 0);
    }
}


void browser_draw_entry(const entry_t *e, int row) {
    const char *delim_str = "  ";
    int i = 1;

    wmove(g_wins[BROWSER].win, row, 1);
    waddch(g_wins[BROWSER].win, ' ');

    // Add note marker
    if (e->note)
	waddch(g_wins[BROWSER].win, '*');
    else
	waddch(g_wins[BROWSER].win, ' ');

    waddch(g_wins[BROWSER].win, ' ');

    draw_date(g_wins[BROWSER].win, e->date, 0);
    draw_str(g_wins[BROWSER].win, e->name, browser_col_widths[i++], 
	delim_str, 0);
    draw_amount(g_wins[BROWSER].win, e->amount, browser_col_widths[i++], 
	delim_str, 1, 0);

    char* cat;
    char* subcat;
    cat_id_to_names(g_categories, e->cat->id, &cat, &subcat);
    draw_str(g_wins[BROWSER].win, cat, browser_col_widths[i++], delim_str, 0);
    if (subcat)
	draw_str(g_wins[BROWSER].win, subcat, browser_col_widths[i++], 
	    delim_str, 0);
    else
	draw_str(g_wins[BROWSER].win, "", browser_col_widths[i++], 
	    delim_str, 0);

    // pad with blank spaces
    int pad_len = getmaxx(g_wins[BROWSER].win) - getcurx(g_wins[BROWSER].win) - 1;
    draw_str(g_wins[BROWSER].win, "", pad_len, "", 0);
}
