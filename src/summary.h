#ifndef SUMMARY_H
#define SUMMARY_H

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

summary_t* init_summary(time_t n_date, delin_t d, int height, int width);
void free_summary(summary_t* s);

void summary_calc();
int summary_update_on_entry(entry_t *e);
void summary_inc_cell(int x, int y, int value);
int summary_set_date_bounds(time_t *n_date, time_t *o_date, delin_t d);
void summary_clear();
void summary_reset(time_t max_date, delin_t d);

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
