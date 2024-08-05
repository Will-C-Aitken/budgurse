#ifndef BUDGURSE_H
#define BUDGURSE_H

#include <ncurses.h>
#include <limits.h>
#include <math.h>
#include "entries.h"

// widths for data, name, amount, category, and subcategory respectively
static const int browser_col_widths[] = {10, 25, 14, 15, 15};

void draw_browser(WINDOW *browser_win, const entry_list_t *el);
void browser_draw_header(WINDOW *browser_win);
void browser_draw_entry(WINDOW *browser_win, const entry_t *e, int row);
void browser_draw_date(WINDOW *browser_win, time_t date, int *col, 
	int max_width);
void browser_draw_amount(WINDOW *browser_win, float amount, int *col,
	int max_width);
void browser_draw_string(WINDOW *browser_win, const char *cat, int *col,
	int max_width);

int num_places_in_amount (int n);

#endif
