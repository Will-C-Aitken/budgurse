#ifndef BUDGURSE_H
#define BUDGURSE_H

#include <ncurses.h>
#include "entries.h"

void draw_browser(WINDOW *browser_win, const entry_list_t *el);
void browser_draw_header(WINDOW *browser_win, int max_col);
void browser_draw_entry(WINDOW *browser_win, const entry_t *e, int row, 
	int max_col);
void browser_draw_date(WINDOW *browser_win, time_t date, int *col);
void browser_draw_amount(WINDOW *browser_win, float amount, int *col);
void browser_draw_string(WINDOW *browser_win, const char *cat, int *col,
	int max_len);

#endif
