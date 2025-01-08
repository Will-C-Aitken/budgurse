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

#include "summary.h"

summary_t *g_summary = NULL;
const char *mnth_hdrs[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


summary_t* init_summary(time_t max_date, delin_t d, int height, int width) {

    EXIT_IF(!g_categories, "Categories must be initialized before summary\n");

    summary_t *s = malloc(sizeof(summary_t));

    // if no supplied date, use current date as newest in summary
    if (!max_date)
	s->max_date = time(NULL);
    else
	s->max_date = max_date;
    int num_cols = summary_set_date_bounds(&s->max_date, &s->min_date, d);

    s->delin = d;

    if (height == -1)
	height = g_wins[SUMMARY].h;
    if (width == -1)
	width = g_wins[SUMMARY].w;
    
    // 1 more for total 
    s->num_cols = num_cols + 1;
    s->num_rows = 0;
    cat_set_sum_idxs(g_categories, &s->num_rows);

    s->cat_array = malloc(sizeof(category_t *) * s->num_rows);
    int temp_idx = 0;
    cat_llist_to_array(g_categories, &s->cat_array, &temp_idx);

    // 1 more for totals
    s->num_rows++;

    // set starting position to end
    s->x_sel = s->x_end = s->num_cols - 1;
    s->y_sel = s->y_end = s->num_rows - 1;

    // limit visible columns to only full columns
    int x_space = ((width - CAT_STR_LEN - 11) / (AMOUNT_STR_LEN + 1));
    s->x_start = (s->num_cols > x_space) ? s->num_cols - x_space : 0;

    // check if room for all categories
    int y_space = (height - 5);
    s->y_start = (s->num_rows > y_space) ? s->num_rows - y_space : 0;
    
    // table for sums
    int n = s->num_rows * s->num_cols;
    s->data = malloc(sizeof(float) * n);
    memset(s->data, 0.00, sizeof(float) * n);

    return s;
}


void free_summary(summary_t* s) {
    if (!s)
	return;
    free(s->cat_array);
    free(s->data);
    free(s);
}


void summary_calc() {
    EXIT_IF(!g_entries, "Entries must be initialized before calculating "
	"summary\n");

    summary_clear();

    if (g_entries->num_nodes == 0)
	return;

    llist_node_t *curr_en = g_entries->tail;
    entry_t *e;

    while (curr_en) {
	e = (entry_t*)curr_en->data;
	// end early if past earliest date in sum 
	if (summary_update_on_entry(e) == -1)
	    return;
	curr_en = curr_en->prev; 
    }
}


// returns -1 if entry is before summary bounds, 1 if beyond, and 0 if in
int summary_update_on_entry(entry_t *e) {
    int x, y;
    int max_x = g_summary->num_cols - 1;
    int max_y = g_summary->num_rows - 1;

    if (e->date < g_summary->min_date)
	return -1; 
    if (e->date > g_summary->max_date)
	return 1;

    x = date_part_from_delin(e->date, g_summary->delin);
    y = e->cat->sum_idx;

    summary_inc_cell(x, y, e->amount);
    // also update column total, row total
    summary_inc_cell(x, max_y, e->amount);
    summary_inc_cell(max_x, y, e->amount);
    // and total total
    summary_inc_cell(max_x, max_y, e->amount);

    // do the same for parent category if it exists
    if (e->cat->p_id != 0) {
	y = cat_get_from_id(g_categories, e->cat->p_id)->sum_idx; 
	summary_inc_cell(x, y, e->amount);
	summary_inc_cell(max_x, y, e->amount);
	// except for column total because that would be a duplicate
    }

    return 0;
}


void summary_inc_cell(int x, int y, float value) {
    EXIT_IF(x >= g_summary->num_cols || y >= g_summary->num_rows, 
	"Invalid summary table idx(s)\n");
    g_summary->data[y*g_summary->num_cols + x] += value;
}


// returns num cols for between max_date and min_date (inclusive)
int summary_set_date_bounds(time_t *max_date, time_t *min_date, delin_t d) {
    struct tm max_tm = *localtime(max_date);
    struct tm min_tm;
    int num_cols;

    switch (d) {
	case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
	case MONTH:
	    num_cols = 12; 
	    max_tm.tm_mday=days_in_mnth[max_tm.tm_mon];
	    min_tm = max_tm;
	    min_tm.tm_mon = max_tm.tm_mon + 1;
	    min_tm.tm_year = max_tm.tm_year - 1;
	    if (min_tm.tm_mon > 11)
		min_tm.tm_year++;
	    min_tm.tm_mon %= 12;
	    min_tm.tm_mday = 1;
	    break;
	case YEAR: EXIT("Year delineation not implemented yet. Exiting\n");
    }

    clean_tm(&max_tm);
    clean_tm(&min_tm);
    *max_date = mktime(&max_tm);
    *min_date = mktime(&min_tm);
    return num_cols; 
}


void summary_reset(time_t max_date, delin_t d) {
    free_summary(g_summary);
    g_summary = init_summary(max_date, d, -1, -1);
}


void summary_clear() {
    int n = g_summary->num_rows * g_summary->num_cols;
    memset(g_summary->data, 0.00, sizeof(float) * n);
}


void summary_draw() {
    int x_start = g_summary->x_start;
    int x_sel = g_summary->x_sel;
    int x_end = g_summary->x_end;
    int nc = g_summary->num_cols;
    int y_start = g_summary->y_start;
    int y_sel = g_summary->y_sel;
    int y_end = g_summary->y_end;
    int nr = g_summary->num_rows;
    int x_idx, y_idx;
    int vert_idx_1 = 21;
    int vert_idx_2 = 21 + 4 + ((x_end - x_start) * (AMOUNT_STR_LEN + 1));
    int row = 3;
    int row_attrs, col_attrs;
    float amt;
    int date_offset = 0;


    werase(g_wins[SUMMARY].win);
    box(g_wins[SUMMARY].win, 0, 0);

    // Top tees for separator v line
    mvwaddch(g_wins[SUMMARY].win, 0, vert_idx_1, ACS_TTEE);
    mvwaddch(g_wins[SUMMARY].win, 0, vert_idx_2, ACS_TTEE);

    summary_draw_header();

    wmove(g_wins[SUMMARY].win, 2, 0);
    waddch(g_wins[SUMMARY].win, ACS_LTEE);
    whline(g_wins[SUMMARY].win, ACS_HLINE, g_wins[SUMMARY].w - 2);
    mvwaddch(g_wins[SUMMARY].win, 2, vert_idx_1, ACS_PLUS);
    mvwaddch(g_wins[SUMMARY].win, 2, vert_idx_2, ACS_PLUS);
    mvwaddch(g_wins[SUMMARY].win, 2, g_wins[SUMMARY].w - 1, ACS_RTEE);

    switch (g_summary->delin) {
	case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
	case MONTH: 
	    date_offset = date_part_from_delin(g_summary->max_date, MONTH);
	    break;
	case YEAR: EXIT("Year delineation not implemented yet. Exiting\n");
    }

    for (int i = y_start; i <= y_end; i++) {
	row_attrs = 0;

	// if at end, insert total hline separator (I hardly know her), and
	// set index to final row regardless of pos (i.e. freeze totals)
	if (i != y_end)
	    y_idx = i;
	else {
	    wmove(g_wins[SUMMARY].win, row, 0);
	    waddch(g_wins[SUMMARY].win, ACS_LTEE);
	    whline(g_wins[SUMMARY].win, ACS_HLINE, g_wins[SUMMARY].w - 2);
	    mvwaddch(g_wins[SUMMARY].win, row, g_wins[SUMMARY].w-1, ACS_RTEE);
	    mvwaddch(g_wins[SUMMARY].win, row, vert_idx_1, ACS_PLUS);
	    mvwaddch(g_wins[SUMMARY].win, row++, vert_idx_2, ACS_PLUS);
	    y_idx = nr - 1;
	}

	wmove(g_wins[SUMMARY].win, row++, 1);

	category_t *c;
	char* cat_name;
	int cat_name_len = CAT_STR_LEN;

	// Total category is not a real category
	if (i == y_end) {
	    cat_name = "Total";
	    row_attrs |= A_ITALIC;
	}
	else {
	    c = g_summary->cat_array[y_idx];
	    // insert space for subcategories
	    if (c->p_id != 0) {
		waddch(g_wins[SUMMARY].win, ' ');
		cat_name_len--;
	    } else 
		row_attrs |= A_BOLD;
	    cat_name = c->name;
	}

	draw_str(g_wins[SUMMARY].win, cat_name, cat_name_len, " ", row_attrs);
	waddch(g_wins[SUMMARY].win, ' ');
	waddch(g_wins[SUMMARY].win, ACS_VLINE);

	for (int j = x_start; j <= x_end; j++) {
	    col_attrs = row_attrs;

	    if (j != x_end)
		x_idx = (j + date_offset + 1) % (nc - 1);
	    else {
		// final column is frozen to be always totals
		x_idx = nc - 1;
		waddstr(g_wins[SUMMARY].win, "   ");
		waddch(g_wins[SUMMARY].win, ACS_VLINE);
	    }

	    amt = g_summary->data[y_idx*nc + x_idx];
	    if (i == y_sel && j == x_sel)
	    	col_attrs |= A_REVERSE;
	    if (i != y_end && j == x_end)
		col_attrs |= A_ITALIC;

	    draw_amount(g_wins[SUMMARY].win, amt, AMOUNT_STR_LEN, " ", 
		col_attrs);
	}
    }
    
    // fill empty rows with just the vertical cat separator

    if (row < g_wins[SUMMARY].h - 1) {
	mvwhline(g_wins[SUMMARY].win, row, 1, ACS_HLINE, 
	    g_wins[SUMMARY].w - 2);
	mvwaddch(g_wins[SUMMARY].win, row, 0, ACS_LTEE);
	mvwaddch(g_wins[SUMMARY].win, row, g_wins[SUMMARY].w-1, ACS_RTEE);
    }

    mvwaddch(g_wins[SUMMARY].win, row, vert_idx_1, ACS_BTEE);
    mvwaddch(g_wins[SUMMARY].win, row, vert_idx_2, ACS_BTEE);


    prompt_default();
    wrefresh(g_wins[SUMMARY].win);
}


void summary_draw_header() {
    int x_start = g_summary->x_start;
    int x_end = g_summary->x_end;
    int curr_yr = date_part_from_delin(g_summary->max_date, YEAR);
    int n_mnth = date_part_from_delin(g_summary->max_date, MONTH);

    mvwaddch(g_wins[SUMMARY].win, 1, CAT_STR_LEN + 3, ACS_VLINE);

    for (int i = x_start; i <= x_end; i++) {
	switch (g_summary->delin) {
	    case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
	    case MONTH: 
		// always print header
		if (i == x_end) {
		    waddstr(g_wins[SUMMARY].win, "   ");
		    waddch(g_wins[SUMMARY].win, ACS_VLINE);
		    summary_draw_mnth_hdr(12, n_mnth, curr_yr);
		} else
		    summary_draw_mnth_hdr(i, n_mnth, curr_yr);
		break;
	    case YEAR: EXIT("Year delineation not implemented yet. Exiting\n");
	}
    }
}


void summary_draw_mnth_hdr(int m, int ref_m, int y) {
    if (m == 12) {
	draw_ra_string(g_wins[SUMMARY].win, "12m Total", AMOUNT_STR_LEN, 
	    " ", A_ITALIC);
	return;
    }

    char hdr[AMOUNT_STR_LEN];
    int hdr_ind = ((m + ref_m + 1) % 12);
    y = (hdr_ind > ref_m) ? y - 1 : y;
    const char **m_str_addr = mnth_hdrs + hdr_ind;

    snprintf(hdr, AMOUNT_STR_LEN, "%s %d", *m_str_addr, y);
    draw_ra_string(g_wins[SUMMARY].win, hdr, AMOUNT_STR_LEN, " ", 0);
}


void summary_redraw_sel_cat(int blink) {
    int y_sel = g_summary->y_sel;

    // cannot total (last) category
    if (y_sel >= g_summary->num_rows - 1)
	return;

    int attrs = 0;
    int y_start = g_summary->y_start;
    category_t *cat = g_summary->cat_array[y_sel];

    wmove(g_wins[SUMMARY].win, y_sel - y_start + 3, 1);
    if (blink)
        attrs |= A_BLINK;

    // draw space in front for subcategory
    if (cat->p_id != 0)
	draw_str(g_wins[SUMMARY].win, " ", 1, "", attrs);
    else
	attrs |= A_BOLD;

    draw_str(g_wins[SUMMARY].win, cat->name, CAT_STR_LEN, " ", attrs);
    wrefresh(g_wins[SUMMARY].win);
}


void summary_scroll(int num_times, dir_t dir) {
    int nc = g_summary->num_cols;
    int nr = g_summary->num_rows;

    while (num_times > 0) {
	switch (dir) {
	    case UP:
		summary_mv_idxs(&g_summary->y_start, &g_summary->y_sel, 
		    &g_summary->y_end, 0, 0);
		break;
	    case DOWN:
		summary_mv_idxs(&g_summary->y_start, &g_summary->y_sel, 
		    &g_summary->y_end, 1, nr-1);
		break;
	    case LEFT:
		summary_mv_idxs(&g_summary->x_start, &g_summary->x_sel, 
		    &g_summary->x_end, 0, 0);
		break;
	    case RIGHT:
		summary_mv_idxs(&g_summary->x_start, &g_summary->x_sel, 
		    &g_summary->x_end, 1, nc-1);
		break;
	}
    num_times--;
    }
}


void summary_mv_idxs(int *i_start, int *i_sel, int *i_end, int pos, int lim) {
    if (!pos) {
	// not everything is visible and at start. need to move edges
	if (*i_start != lim && *i_start == *i_sel) {
	    (*i_start)--;
	    (*i_end)--;
	}
	*i_sel = (*i_sel == lim) ? *i_sel : *i_sel - 1;
    } else {
	// not everything is visible and one away from end. need to move edges 	
	if (*i_end != lim && *i_sel + 1 >= *i_end) {
	    (*i_start)++;
	    (*i_end)++;
	}
	// (but don't move sel to frozen last element until the very end)
	if (*i_sel != lim)
	    (*i_sel)++;
    }
}


void summary_edit_category() {
    if (g_summary->y_sel >= g_summary->num_rows - 1) {
	prompt_display("Cannot edit Total category", 0, 1);
	return;
    }
    summary_redraw_sel_cat(1);
    prompt_edit_category(g_summary->cat_array[g_summary->y_sel]);
}


void summary_del_category() {
    if (g_summary->y_sel >= g_summary->num_rows - 1) {
	prompt_display("Cannot delete Total category", 0, 1);
	return;
    }

    category_t *sel_cat = g_summary->cat_array[g_summary->y_sel];

    if (sel_cat->subcats) {
	prompt_display("Delete subcategories first", 0, 1);
	return;
    }

    // handles changing the existing entries w/ category
    if (!prompt_del_category(sel_cat))
	return;

    db_exec(sel_cat, (gen_sql_fn_t)del_cat_to_sql);
    cat_del_from_llist(g_categories, sel_cat);

    summary_reset(g_summary->max_date, g_summary->delin);
    summary_calc();
}


int summary_handle_key(int ch) {
    EXIT_IF(!g_summary, "Summary not initialized");

    switch (ch) {
	case '0':
	    summary_scroll(g_summary->num_cols, LEFT);
	    break;
	case 'b':
	    state = BROWSER;
	    break;
	case 'd':
	    summary_del_category();
	    break;
	case 'e':
	    summary_edit_category();
	    break;
	case 'g':
	    if (wgetch(g_wins[SUMMARY].win) == 'g')
		summary_scroll(g_summary->num_rows, UP);
	    break;
	case 'G':
	    summary_scroll(g_summary->num_rows, DOWN);
	    break;
	case 'h':
	case KEY_LEFT:
	    summary_scroll(1, LEFT);
	    break;
	case 'j':
	case KEY_DOWN:
	    summary_scroll(1, DOWN);
	    break;
	case 'k':
	case KEY_UP:
	    summary_scroll(1, UP);
	    break;
	case 'l':
	case KEY_RIGHT:
	    summary_scroll(1, RIGHT);
	    break;
	case 'm':
	    update_date(&g_summary->min_date, MONTH, 1);
	    update_date(&g_summary->max_date, MONTH, 1);
	    summary_calc();
	    break;
	case 'M':
	    update_date(&g_summary->min_date, MONTH, -1);
	    update_date(&g_summary->max_date, MONTH, -1);
	    summary_calc();
	    break;
	case 'q':
	    return 0;
	case 'y':
	    update_date(&g_summary->min_date, YEAR, 1);
	    update_date(&g_summary->max_date, YEAR, 1);
	    summary_calc();
	    break;
	case 'Y':
	    update_date(&g_summary->min_date, YEAR, -1);
	    update_date(&g_summary->max_date, YEAR, -1);
	    summary_calc();
	    break;
	case '?':
	    g_help = init_help(state);
	    state = HELP;
	    break;
	case '$':
	    summary_scroll(g_summary->num_cols, RIGHT);
	    break;
    }
    return 1;
}
