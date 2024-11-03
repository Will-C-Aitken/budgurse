#ifndef SUMMARY_H
#define SUMMARY_H

#include "budgurse.h"

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
} summary_t;

extern summary_t *g_summary;

summary_t* init_summary(delin_t d, int height, int width);
void calc_summary();
void summary_update_cell(entry_node_t *en, int max_date_part);
void free_summary(summary_t* s);

int summary_handle_key(int ch);

int date_part_from_delin(time_t date, delin_t d);

#endif
