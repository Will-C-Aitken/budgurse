#include "test.h"


entry_list_t* test_dummy_entry_list(int num_nodes) {
    entry_list_t *el = init_entry_list();
    for (int i = 1; i <= num_nodes; i++)
	insert_entry(el, init_entry_node(test_dummy_entry(i)), to_tail);
    return el;
}


entry_t *test_dummy_entry(int id) {
    int category_id = 1;
    struct tm test_time_tm = {.tm_sec = 0,
			       .tm_min = 0,
			       .tm_hour = 0,
			       .tm_mday = 12,
			       .tm_mon = 1, // 0-based so 6 is July
			       .tm_year = 2022 - 1900, // year starts at 1900
			       .tm_isdst = 1}; // daylight savings?
    time_t test_date = mktime(&test_time_tm);
    return init_entry(id, "A Name", test_date, -12.00, category_id, "A Note");
}


// Add categories alternating between main and sub
cat_array_t* test_dummy_cat_array(int num_cats) {
    cat_array_t *ca = init_cat_array();
    category_t *c = NULL;
    char cat_name[CAT_STR_LEN+1];
    for (int i = 1; i <= num_cats; i++) {
	snprintf(cat_name, CAT_STR_LEN, "Cat%d", i); 
	if (i % 2)
	    c = init_category(i, 0, cat_name);
	else
	    c = init_category(i, i-1, cat_name);
	append_to_cat_array(ca, c);
    }
    return ca;
}


int is_amount_eq(float candidate, float actual) {
    // less than half a cent off is fine
    float e = 0.005;
    return (abs(candidate - actual) < e);
}
