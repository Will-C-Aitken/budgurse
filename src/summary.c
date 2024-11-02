#include "summary.h"

summary_t *g_summary = NULL;


summary_t* init_summary(delin_t d, int height, int width) {

    EXIT_IF(!g_categories, "Categories must be initialized before summary\n");
    
    int num_cols;
    switch (d) {
	case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
	case MONTH: num_cols = 12; break;
	case YEAR: EXIT("Year delineation not implemented yet. Exiting\n");
    }

    summary_t *s = malloc(sizeof(summary_t));

    if (height == -1)
	height = g_wins[SUMMARY].h;
    if (width == -1)
	width = g_wins[SUMMARY].w;
    
    // 1 more for total(s) and 4 less for header and borders
    s->num_rows = g_categories->num_cats + 1;
    // 2 more for totals and avg
    s->num_cols = num_cols + 2;

    // set starting position to end
    s->x_sel = s->x_end = s->num_cols - 1;
    s->y_sel = s->y_end = s->num_rows - 1;

    // limit visible columns to only full columns
    s->x_start = s->num_cols - ((width - CAT_STR_LEN - 7) 
				/ (AMOUNT_STR_LEN + 1)) - 1;
    // check if room for all categories
    int y_space = (height - 4);
    s->y_start = (s->num_rows > y_space) ? s->num_rows - y_space : 0;
    
    // table for sums
    int n = s->num_rows * s->num_cols;
    s->data = malloc(sizeof(float) * n);
    memset(s->data, 0.00, sizeof(float) * n);
    return s;
}


void calc_summary(summary_t *s, entry_list_t* el, bool sorted_by_date) {
    EXIT_IF(!el, "Passed entry list not initialized\n");
}


void free_summary(summary_t* s) {
    if (!s)
	return;
    free(s->data);
    free(s);
}


int summary_handle_key(int ch) {
    return 1;
}
