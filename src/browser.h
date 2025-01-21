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

#ifndef BROWSER_H
#define BROWSER_H

#include "llist.h"
#include "categories.h"
#include "entries.h"
#include "backend.h"
#include "prompt.h"
#include "summary.h"
#include "help.h"
#include "wins.h"
#include "util.h"
#include "status.h"
#include "global.h"
#include "budgurse.h"

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

browser_t* init_browser(llist_t *el, llist_node_t *sel_node, int sel_pos, 
	int max_num_entries);
void free_browser(browser_t* b);

int browser_handle_key(int ch);
void browser_scroll(int num_times, dir_t dir);
void browser_to_top();
void browser_add_entry();
void browser_insert(llist_node_t *en);
void browser_edit_entry();
void browser_del_entry();
llist_node_t *browser_pop_sel_entry();
void browser_view_sel_entry();

// drawing functions
void browser_draw();
void browser_draw_header();
void browser_draw_entry(const entry_t *e, int row);

#endif
