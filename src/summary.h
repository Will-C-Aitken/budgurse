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

#ifndef SUMMARY_H
#define SUMMARY_H

#include "entries.h"
#include "categories.h"
#include "wins.h"
#include "browser.h"
#include "util.h"
#include "status.h"
#include "global.h"
#include "budgurse.h"

extern const char *mnth_hdrs[];

typedef struct summary {
    delin_t delin;
    int num_cols;
    int num_rows;
    float *data;
    int x_start;
    int x_sel;
    int x_end;
    int y_start;
    int y_sel;
    int y_end;
    time_t min_date;
    time_t max_date;
    category_t **cat_array;
} summary_t;

extern summary_t *g_summary;

summary_t* init_summary(time_t n_date, delin_t d, int height, int width,
	int sel_x, int sel_y);
void free_summary(summary_t* s);

void summary_calc();
int summary_update_on_entry(entry_t *e);
void summary_inc_cell(int x, int y, float value);
int summary_set_date_bounds(time_t *n_date, time_t *o_date, delin_t d);
void summary_clear();
void summary_reset(time_t max_date, delin_t d, int cur_x, int cur_y);
void summary_resize();

void summary_draw();
void summary_draw_header();
void summary_draw_mnth_hdr(int m, int ref_m, int y);
void summary_redraw_sel_cat(int blink);

void summary_scroll(int num_times, dir_t dir);
void summary_mv_idxs(int *i_start, int *i_sel, int *i_end, int amt, int lim);

void summary_edit_category();
void summary_del_category();

int summary_handle_key(int ch);

#endif
