#ifndef BROWSER_H
#define BROWSER_H

#include "budgurse.h"

#define DATE_STR_LEN 10
#define NAME_STR_LEN 22
#define AMOUNT_STR_LEN 11
#define CAT_STR_LEN 18
#define SUBCAT_STR_LEN 18

static const int browser_col_widths[] = {DATE_STR_LEN, NAME_STR_LEN, 
    AMOUNT_STR_LEN, CAT_STR_LEN, SUBCAT_STR_LEN};

// subset of entire entry list currently being displayed
typedef struct browser {
    llist_node_t* start;
    llist_node_t* sel;
    llist_node_t* end;
    int num_entries;
    int max_num_entries;
} browser_t;

extern browser_t *g_browser;

browser_t* init_browser(llist_t* el, int max_num_entries);
void free_browser(browser_t* b);

int browser_handle_key(int ch);
void browser_scroll(int num_times, dir_t dir);
void browser_add_entry();
void browser_insert(llist_node_t *en);
void browser_edit_entry();
void browser_del_entry();
llist_node_t *browser_pop_sel_entry();

// drawing functions
void browser_draw();
void browser_draw_header();
void browser_draw_entry(const entry_t *e, int row);
void browser_draw_date(time_t date, int max_width);

#endif
