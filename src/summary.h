#ifndef SUMMARY_H
#define SUMMARY_H

#include "budgurse.h"

typedef enum delin {
    WEEK,
    MONTH,
    YEAR,
} delin_t;

typedef struct summary {
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
void calc_summary(summary_t *s, entry_list_t* el, bool sorted_by_date);
void free_summary(summary_t* s);

int summary_handle_key(int ch);

#endif
