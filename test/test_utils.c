#include "test.h"


entry_list_t* test_dummy_entry_list(int num_nodes) {
    entry_list_t *el = init_entry_list();
    for (int i = 1; i <= num_nodes; i++)
	append_to_tail(el, init_entry_node(test_dummy_entry(i)));
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


int is_amount_eq(float candidate, float actual) {
    // less than half a cent off is fine
    float e = 0.005;
    return (abs(candidate - actual) < e);
}
