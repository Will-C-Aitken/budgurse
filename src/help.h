#ifndef HELP_H
#define HELP_H

#include "wins.h"
#include "util.h"
#include "global.h"
#include "budgurse.h"

typedef struct help {
    const char **txt_table;
    state_t prior_state;
    int first_row_idx;
    int num_rows;
} help_t;

extern help_t *g_help;

help_t *init_help(state_t s);
void free_help(help_t *h);

void help_draw();
void help_scroll(dir_t d);
int help_handle_key(int ch);

#endif
