#include "summary.h"

summary_t *g_summary = NULL;


summary_t* init_summary(entry_list_t* el, delin_t d, int height, int width) {

    EXIT_IF(!el, "Passed entry list not initialized\n");
    EXIT_IF(!g_categories, "Categories must be initialized before summary\n");
    
    int num_cols;
    switch (d) {
	case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
	case MONTH: num_cols = 12; break;
	case YEAR: EXIT("Year delineation not implemented yet. Exiting\n");
    }

    summary_t *s = malloc(sizeof(summary_t));
    
    // 1 more for total(s)
    s->num_rows = g_categories->num_cats + 1;
    s->num_cols = num_cols;

    // set starting position to end
    s->x_sel = s->x_end = s->num_cols;
    s->y_sel = s->y_end = s->num_rows;

    // sort out math
    s->x_start = s->num_cols - ((width - CAT_STR_LEN - 2) 
				% (AMOUNT_STR_LEN + 2));
    s->y_start = s->num_rows - height;
    
    s->data = NULL;

    return s;
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

