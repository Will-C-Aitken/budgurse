#include "test.h"

int entries_tests() {
    mu_run_test(append_to_tail_test);
    mu_run_test(del_tail_test);
    mu_run_test(free_entry_list_test);
    mu_run_test(where_in_list_test);
    mu_run_test(is_end_node_test);
    mu_run_test(del_head_test);
    mu_run_test(del_entry_test);
    return 0;
}

int append_to_tail_test() {

    g_entries = init_entry_list();
    entry_node_t *en = init_entry_node(test_dummy_entry(1));
    append_to_tail(g_entries, en);

    // there should be no next or prev
    mu_assert(!g_entries->head->prev, "Entries", 1);
    mu_assert(!g_entries->head->next, "Entries", 2);
    mu_assert(!g_entries->tail->next, "Entries", 3);
    mu_assert(!g_entries->tail->prev, "Entries", 4);
    mu_assert(g_entries->num_nodes == 1, "Entries", 5);

    // head and tail are same
    mu_assert(g_entries->tail == en, "Entries", 6);
    mu_assert(g_entries->head == en, "Entries", 7);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    append_to_tail(g_entries, en2);

    mu_assert(!g_entries->head->prev, "Entries", 8);
    mu_assert(g_entries->head->next == g_entries->tail, "Entries", 9);
    mu_assert(!g_entries->tail->next, "Entries", 10);
    mu_assert(g_entries->tail->prev == g_entries->head, "Entries", 11);

    mu_assert(g_entries->head == en, "Entries", 12);
    mu_assert(g_entries->tail == en2, "Entries", 13);

    free_entry_list(g_entries);

    return 0;
}
 

int del_tail_test() {

    // Create list of 2 entries
    g_entries = test_dummy_entry_list(2);

    // sec, min, hour, day, month, year, is_dst
    del_tail(g_entries);
    mu_assert(g_entries->num_nodes == 1, "Entries", 14);
    mu_assert(!g_entries->tail->next, "Entries", 15);
    mu_assert(!g_entries->tail->prev, "Entries", 16);
    mu_assert(!g_entries->head->next, "Entries", 17);
    mu_assert(!g_entries->head->prev, "Entries", 18);
    mu_assert(g_entries->head == g_entries->tail, "Entries", 19);

    del_tail(g_entries);
    mu_assert(g_entries->num_nodes == 0, "Entries", 20);
    mu_assert(!g_entries->head, "Entries", 21);
    mu_assert(!g_entries->tail, "Entries", 22);

    del_tail(g_entries);
    free(g_entries);
    return 0;
}


int free_entry_list_test() {
    g_entries = test_dummy_entry_list(2);
    free_entry_list(g_entries);
    return 0;
}


int where_in_list_test() {
    int result;
    g_entries = init_entry_list();

    entry_t* e1 = test_dummy_entry(1);

    // empty list, so en1 can't be in it
    result = where_in_list(g_entries, e1);
    mu_assert(result == -1, "Entries", 23);

    append_to_tail(g_entries, init_entry_node(e1));

    // en1 is in list at position 0
    result = where_in_list(g_entries, e1);
    mu_assert(result == 0, "Entries", 24);

    entry_t *e2 = test_dummy_entry(2);

    // en2 is not in list
    result = where_in_list(g_entries, e2);
    mu_assert(result == -1, "Entries", 25);

    append_to_tail(g_entries, init_entry_node(e2));

    // en1 is still in list 
    result = where_in_list(g_entries, e1);
    mu_assert(result == 0, "Entries", 26);

    // en2 is now in list at position 1
    result = where_in_list(g_entries, e2);
    mu_assert(result == 1, "Entries", 27);

    free_entry_list(g_entries);

    return 0;
}


int is_end_node_test() {

    g_entries = init_entry_list();
    entry_node_t* en1 = init_entry_node(test_dummy_entry(1));
    append_to_tail(g_entries, en1);

    mu_assert(is_end_node(en1), "Entries", 28);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    append_to_tail(g_entries, en2);

    // Both are still end nodes
    mu_assert(is_end_node(en2), "Entries", 29);
    mu_assert(is_end_node(en1), "Entries", 30);

    entry_node_t* en3 = init_entry_node(test_dummy_entry(3));
    append_to_tail(g_entries, en3);

    // 1 and 3 are end nodes but 2 isn't
    mu_assert(is_end_node(en1), "Entries", 31);
    mu_assert(is_end_node(en3), "Entries", 32);
    mu_assert(!is_end_node(en2), "Entries", 33);
    mu_assert(is_head(en1), "Entries", 34);
    mu_assert(is_tail(en3), "Entries", 35);

    free_entry_list(g_entries);
    return 0;
}


int entry_node_traverse_test() {
    int cat_id = 1;
    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en1 = init_entry_node(e1);
    append_to_tail(g_entries, en1);

    entry_t* e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id,
	"Zionist scum!");
    entry_node_t* en2 = init_entry_node(e2);
    append_to_tail(g_entries, en2);

    entry_t* e3 = init_entry(3, "Starbucks", d1, -0.31, cat_id,
	"Zionist scum!");
    entry_node_t* en3 = init_entry_node(e3);
    append_to_tail(g_entries, en3);

    entry_node_t* temp = g_entries->tail;

    // temp should stay at node 3
    entry_node_traverse(&temp, DOWN);
    mu_assert(temp == en3, "Entries", 36);

    entry_node_traverse(&temp, UP);
    mu_assert(temp == en2, "Entries", 37);

    entry_node_traverse(&temp, UP);
    mu_assert(temp == en1, "Entries", 38);

    // temp should stay at node 1
    entry_node_traverse(&temp, UP);
    mu_assert(temp == en1, "Entries", 39);

    free_entry_list(g_entries);
    return 0;
}


int del_head_test() {

    g_entries = init_entry_list();
    entry_node_t* en1 = init_entry_node(test_dummy_entry(1));
    append_to_tail(g_entries, en1);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    append_to_tail(g_entries, en2);
    
    entry_node_t* en3 = init_entry_node(test_dummy_entry(3));
    append_to_tail(g_entries, en3);

    del_head(g_entries);
    mu_assert(en2 == g_entries->head, "Entries", 40);
    mu_assert(en2->prev == NULL, "Entries", 41);
    mu_assert(g_entries->num_nodes == 2, "Entries", 42);

    del_head(g_entries);
    mu_assert(g_entries->head == en3, "Entries", 43);
    mu_assert(g_entries->tail == en3, "Entries", 44);
    mu_assert(en3->prev == NULL, "Entries", 45);
    mu_assert(g_entries->num_nodes == 1, "Entries", 46);

    del_head(g_entries);
    mu_assert(g_entries->head == NULL, "Entries", 47);
    mu_assert(g_entries->tail == NULL, "Entries", 48);
    mu_assert(g_entries->num_nodes == 0, "Entries", 49);

    free_entry_list(g_entries);
    return 0;
}


int del_entry_test() {

    g_entries = init_entry_list();
    entry_node_t* en1 = init_entry_node(test_dummy_entry(1));
    append_to_tail(g_entries, en1);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    append_to_tail(g_entries, en2);
    
    entry_node_t* en3 = init_entry_node(test_dummy_entry(3));
    append_to_tail(g_entries, en3);

    del_entry(g_entries, en2);
    mu_assert(en1->next == en3, "Entries", 51);
    mu_assert(en3->prev == en1, "Entries", 52);
    mu_assert(g_entries->num_nodes == 2, "Entries", 53);

    del_entry(g_entries, en3);
    mu_assert(g_entries->head == en1, "Entries", 54);
    mu_assert(g_entries->tail == en1, "Entries", 55);
    mu_assert(en1->next == NULL, "Entries", 56);
    mu_assert(en1->prev == NULL, "Entries", 57);
    mu_assert(g_entries->num_nodes == 1, "Entries", 58);

    del_entry(g_entries, en1);
    mu_assert(g_entries->head == NULL, "Entries", 59);
    mu_assert(g_entries->tail == NULL, "Entries", 60);
    mu_assert(g_entries->num_nodes == 0, "Entries", 61);

    free_entry_list(g_entries);
    return 0;
}

