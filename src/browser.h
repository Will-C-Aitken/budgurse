#ifndef BUDGURSE_H
#define BUDGURSE_H

#include <ncurses.h>
#include <limits.h>
#include <math.h>
#include "entries.h"

// widths for data, name, amount, category, and subcategory respectively
static const int browser_col_widths[] = {10, 25, 14, 15, 15};

// subset of entire entry list currently being displayed
typedef struct browser {
    entry_node_t* start;
    entry_node_t* sel;
    entry_node_t* end;
    int num_entries;
    int max_num_entries;
} browser_t;

int browser_handle_key(browser_t* b, int ch);

browser_t* init_browser(const entry_list_t *el, int num_browser_rows);
void browser_scroll(browser_t* b, int num_times, direction_t dir);
void free_browser(browser_t* b);

// drawing functions
void draw_browser(WINDOW *browser_win, const browser_t *b);
void browser_draw_header(WINDOW *browser_win);
void browser_draw_entry(WINDOW *browser_win, const entry_t *e, int row);
void browser_draw_date(WINDOW *browser_win, time_t date, int *col, 
	int max_width);
void browser_draw_amount(WINDOW *browser_win, float amount, int *col,
	int max_width);
void browser_draw_string(WINDOW *browser_win, const char *cat, int *col,
	int max_width);

// utility functions
int num_places_in_amount (int n);
int browser_max_entries(int num_nodes_in_list, int num_rows_in_window);

#endif
