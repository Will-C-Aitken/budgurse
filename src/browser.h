#ifndef BROWSER_H
#define BROWSER_H

#include "budgurse.h"

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

extern browser_t *g_browser;

browser_t* init_browser(entry_list_t* el, int max_num_entries);
int browser_handle_key(int ch);
void browser_scroll(int num_times, direction_t dir);
void free_browser(browser_t* b);

// drawing functions
void draw_browser();
void browser_draw_header();
void browser_draw_entry(const entry_t *e, int row);
void browser_draw_date(time_t date, int *col, int max_width);
void browser_draw_amount(float amount, int *col, int max_width);
void browser_draw_string(const char *cat, int *col, int max_width);

// utility functions
int num_places_in_amount (int n);

#endif
