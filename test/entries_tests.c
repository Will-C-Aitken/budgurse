#include "test.h"

int entries_tests() {
    mu_run_test(insert_after_date_test);
    return 0;
}


int insert_after_date_test() {

    g_entries = init_llist();

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=12, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, NULL, "A Note");
    llist_node_t *en1= init_llist_node(e1);
    llist_insert_node(g_entries, en1, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 1, "Entries", 1);
    mu_assert(g_entries->head == en1, "Entries", 2);
    mu_assert(g_entries->tail == en1, "Entries", 3);

    // two days newer
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=14, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e2 = init_entry(2, "A Name", date2, -12.00, NULL, "A Note");
    llist_node_t *en2= init_llist_node(e2);
    llist_insert_node(g_entries, en2, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 2, "Entries", 4);
    mu_assert(g_entries->head == en1, "Entries", 5);
    mu_assert(g_entries->head->next == en2, "Entries",6);
    mu_assert(g_entries->tail == en2, "Entries", 7);
    mu_assert(g_entries->tail->prev == en1, "Entries", 8);

    // same time
    entry_t *e3 = init_entry(3, "A Name", date2, -12.00, NULL, "A Note");
    llist_node_t *en3= init_llist_node(e3);
    llist_insert_node(g_entries, en3, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 3, "Entries", 9);
    mu_assert(g_entries->head == en1, "Entries", 10);
    mu_assert(g_entries->head->next == en2, "Entries", 11);
    mu_assert(g_entries->tail == en3, "Entries", 12);
    mu_assert(g_entries->tail->prev == en2, "Entries", 13);
    mu_assert(en2->next == en3, "Entries", 14);
    mu_assert(en2->prev == en1, "Entries", 15);

    // one day before (should be second now)
    struct tm tm4 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=13, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date4 = mktime(&tm4);
    entry_t *e4 = init_entry(4, "A Name", date4, -12.00, NULL, "A Note");
    llist_node_t *en4= init_llist_node(e4);
    llist_insert_node(g_entries, en4, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 4, "Entries", 16);
    mu_assert(g_entries->head == en1, "Entries", 17);
    mu_assert(g_entries->head->next == en4, "Entries", 18);
    mu_assert(en4->next == en2, "Entries", 19);
    mu_assert(en4->prev == en1, "Entries", 20);
    mu_assert(en2->prev == en4, "Entries", 21);
    
    // one year before (should be first)
    struct tm tm5 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=13, .tm_mon=0, .tm_year=2021 - 1900, .tm_isdst=1}; 
    time_t date5 = mktime(&tm5);
    entry_t *e5 = init_entry(5, "A Name", date5, -12.00, NULL, "A Note");
    llist_node_t *en5= init_llist_node(e5);
    llist_insert_node(g_entries, en5, (llist_comp_fn_t)entry_date_comp_gte);

    mu_assert(g_entries->num_nodes == 5, "Entries", 22);
    mu_assert(g_entries->head == en5, "Entries", 23);
    mu_assert(g_entries->head->next == en1, "Entries", 24);
    mu_assert(en1->prev == en5, "Entries", 25);

    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    return 0;
}
