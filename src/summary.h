#ifndef SUMMARY_H
#define SUMMARY_H

#include "budgurse.h"

extern const char *mnth_hdrs[];

typedef enum delin {
    WEEK,
    MONTH,
    YEAR,
} delin_t;

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
    time_t o_date;
    time_t n_date;
    char **cat_name_list;
} summary_t;

extern summary_t *g_summary;

summary_t* init_summary(delin_t d, int height, int width);
void free_summary(summary_t* s);

void summary_calc();
int summary_update_on_entry(entry_t *e);
void summary_inc_cell(int x, int y, int value);
void summary_set_date_bounds(time_t n_date);
void summary_clear();
void summary_reset(delin_t d);

void summary_draw();
void summary_draw_header();
void summary_draw_mnth_hdr(int m, int ref_m, int y);

void summary_scroll(int num_times, dir_t dir);
void summary_mv_idxs(int *i_start, int *i_sel, int *i_end, int amt, int lim);

int summary_handle_key(int ch);

int date_part_from_delin(time_t date, delin_t d);

#endif
