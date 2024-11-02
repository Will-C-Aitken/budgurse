#include "test.h"

int entries_tests() {
    mu_run_test(append_to_tail_test);
    mu_run_test(del_tail_test);
    mu_run_test(free_entry_list_test);
    mu_run_test(where_in_list_test);
    mu_run_test(is_end_node_test);
    mu_run_test(del_head_test);
    mu_run_test(del_entry_test);
    mu_run_test(insert_after_date_test);
    mu_run_test(dist_between_test);
    return 0;
}

int append_to_tail_test() {

    g_entries = init_entry_list();
    entry_node_t *en = init_entry_node(test_dummy_entry(1));
    insert_entry(g_entries, en, to_tail);

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
    insert_entry(g_entries, en2, to_tail);

    mu_assert(!g_entries->head->prev, "Entries", 8);
    mu_assert(g_entries->head->next == g_entries->tail, "Entries", 9);
    mu_assert(!g_entries->tail->next, "Entries", 10);
    mu_assert(g_entries->tail->prev == g_entries->head, "Entries", 11);

    mu_assert(g_entries->head == en, "Entries", 12);
    mu_assert(g_entries->tail == en2, "Entries", 13);

    entry_node_t* en3 = init_entry_node(test_dummy_entry(2));
    insert_entry(g_entries, en3, to_tail);

    mu_assert(!g_entries->head->prev, "Entries", 14);
    mu_assert(g_entries->head->next == en2, "Entries", 15);
    mu_assert(!g_entries->tail->next, "Entries", 16);
    mu_assert(g_entries->tail->prev == en2, "Entries", 17);

    mu_assert(en2->next == en3, "Entries", 18);
    mu_assert(en2->prev == en, "Entries", 19);

    free_entry_list(g_entries);

    return 0;
}
 

int del_tail_test() {

    // Create list of 2 entries
    g_entries = test_dummy_entry_list(2);

    // sec, min, hour, day, month, year, is_dst
    del_tail(g_entries);
    mu_assert(g_entries->num_nodes == 1, "Entries", 20);
    mu_assert(!g_entries->tail->next, "Entries", 21);
    mu_assert(!g_entries->tail->prev, "Entries", 22);
    mu_assert(!g_entries->head->next, "Entries", 23);
    mu_assert(!g_entries->head->prev, "Entries", 24);
    mu_assert(g_entries->head == g_entries->tail, "Entries", 25);

    del_tail(g_entries);
    mu_assert(g_entries->num_nodes == 0, "Entries", 26);
    mu_assert(!g_entries->head, "Entries", 27);
    mu_assert(!g_entries->tail, "Entries", 28);

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
    mu_assert(result == -1, "Entries", 29);

    insert_entry(g_entries, init_entry_node(e1), to_tail);

    // en1 is in list at position 0
    result = where_in_list(g_entries, e1);
    mu_assert(result == 0, "Entries", 30);

    entry_t *e2 = test_dummy_entry(2);

    // en2 is not in list
    result = where_in_list(g_entries, e2);
    mu_assert(result == -1, "Entries", 31);

    insert_entry(g_entries, init_entry_node(e2), to_tail);

    // en1 is still in list 
    result = where_in_list(g_entries, e1);
    mu_assert(result == 0, "Entries", 32);

    // en2 is now in list at position 1
    result = where_in_list(g_entries, e2);
    mu_assert(result == 1, "Entries", 33);

    free_entry_list(g_entries);

    return 0;
}


int is_end_node_test() {

    g_entries = init_entry_list();
    entry_node_t* en1 = init_entry_node(test_dummy_entry(1));
    insert_entry(g_entries, en1, to_tail);

    mu_assert(is_end_node(en1), "Entries", 34);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    insert_entry(g_entries, en2, to_tail);

    // Both are still end nodes
    mu_assert(is_end_node(en2), "Entries", 35);
    mu_assert(is_end_node(en1), "Entries", 36);

    entry_node_t* en3 = init_entry_node(test_dummy_entry(3));
    insert_entry(g_entries, en3, to_tail);

    // 1 and 3 are end nodes but 2 isn't
    mu_assert(is_end_node(en1), "Entries", 37);
    mu_assert(is_end_node(en3), "Entries", 38);
    mu_assert(!is_end_node(en2), "Entries", 39);
    mu_assert(is_head(en1), "Entries", 40);
    mu_assert(is_tail(en3), "Entries", 41);

    free_entry_list(g_entries);
    return 0;
}


int entry_node_traverse_test() {
    int cat_id = 1;
    g_entries = init_entry_list();

    struct tm tm1 = {.tm_mday=12, .tm_mon=1, .tm_year=2022 - 1900};
    time_t d1 = mktime(&tm1);
    entry_t* e1 = init_entry(1, "Starbucks", d1, -0.21, cat_id,
	"I might have a strong opinion about this");
    entry_node_t* en1 = init_entry_node(e1);
    insert_entry(g_entries, en1, to_tail);

    entry_t* e2 = init_entry(2, "Starbucks", d1, -0.21, cat_id,
	"I might have a strong opinion about this");
    entry_node_t* en2 = init_entry_node(e2);
    insert_entry(g_entries, en2, to_tail);

    entry_t* e3 = init_entry(3, "Starbucks", d1, -0.31, cat_id,
	"I might have a strong opinion about this");
    entry_node_t* en3 = init_entry_node(e3);
    insert_entry(g_entries, en3, to_tail);

    entry_node_t* temp = g_entries->tail;

    // temp should stay at node 3
    entry_node_traverse(&temp, DOWN);
    mu_assert(temp == en3, "Entries", 42);

    entry_node_traverse(&temp, UP);
    mu_assert(temp == en2, "Entries", 43);

    entry_node_traverse(&temp, UP);
    mu_assert(temp == en1, "Entries", 44);

    // temp should stay at node 1
    entry_node_traverse(&temp, UP);
    mu_assert(temp == en1, "Entries", 45);

    free_entry_list(g_entries);
    return 0;
}


int del_head_test() {

    g_entries = init_entry_list();
    entry_node_t* en1 = init_entry_node(test_dummy_entry(1));
    insert_entry(g_entries, en1, to_tail);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    insert_entry(g_entries, en2, to_tail);
    
    entry_node_t* en3 = init_entry_node(test_dummy_entry(3));
    insert_entry(g_entries, en3, to_tail);

    del_head(g_entries);
    mu_assert(en2 == g_entries->head, "Entries", 46);
    mu_assert(en2->prev == NULL, "Entries", 47);
    mu_assert(g_entries->num_nodes == 2, "Entries", 48);

    del_head(g_entries);
    mu_assert(g_entries->head == en3, "Entries", 49);
    mu_assert(g_entries->tail == en3, "Entries", 50);
    mu_assert(en3->prev == NULL, "Entries", 51);
    mu_assert(g_entries->num_nodes == 1, "Entries", 52);

    del_head(g_entries);
    mu_assert(g_entries->head == NULL, "Entries", 53);
    mu_assert(g_entries->tail == NULL, "Entries", 54);
    mu_assert(g_entries->num_nodes == 0, "Entries", 55);

    free_entry_list(g_entries);
    return 0;
}


int del_entry_test() {

    g_entries = init_entry_list();
    entry_node_t* en1 = init_entry_node(test_dummy_entry(1));
    insert_entry(g_entries, en1, to_tail);

    entry_node_t* en2 = init_entry_node(test_dummy_entry(2));
    insert_entry(g_entries, en2, to_tail);
    
    entry_node_t* en3 = init_entry_node(test_dummy_entry(3));
    insert_entry(g_entries, en3, to_tail);

    del_entry(g_entries, en2);
    mu_assert(en1->next == en3, "Entries", 57);
    mu_assert(en3->prev == en1, "Entries", 58);
    mu_assert(g_entries->num_nodes == 2, "Entries", 59);

    del_entry(g_entries, en3);
    mu_assert(g_entries->head == en1, "Entries", 60);
    mu_assert(g_entries->tail == en1, "Entries", 61);
    mu_assert(en1->next == NULL, "Entries", 62);
    mu_assert(en1->prev == NULL, "Entries", 63);
    mu_assert(g_entries->num_nodes == 1, "Entries", 64);

    del_entry(g_entries, en1);
    mu_assert(g_entries->head == NULL, "Entries", 65);
    mu_assert(g_entries->tail == NULL, "Entries", 66);
    mu_assert(g_entries->num_nodes == 0, "Entries", 67);

    free_entry_list(g_entries);
    return 0;
}


int insert_after_date_test() {

    g_entries = init_entry_list();

    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=12, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date1 = mktime(&tm1);
    entry_t *e1 = init_entry(1, "A Name", date1, -12.00, 1, "A Note");
    entry_node_t *en1= init_entry_node(e1);
    insert_entry(g_entries, en1, after_date);

    mu_assert(g_entries->num_nodes == 1, "Entries", 68);
    mu_assert(g_entries->head == en1, "Entries", 69);
    mu_assert(g_entries->tail == en1, "Entries", 70);

    // two days newer
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=14, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    entry_t *e2 = init_entry(2, "A Name", date2, -12.00, 1, "A Note");
    entry_node_t *en2= init_entry_node(e2);
    insert_entry(g_entries, en2, after_date);

    mu_assert(g_entries->num_nodes == 2, "Entries", 71);
    mu_assert(g_entries->head == en1, "Entries", 72);
    mu_assert(g_entries->head->next == en2, "Entries", 73);
    mu_assert(g_entries->tail == en2, "Entries", 74);
    mu_assert(g_entries->tail->prev == en1, "Entries", 75);

    // same time
    entry_t *e3 = init_entry(3, "A Name", date2, -12.00, 1, "A Note");
    entry_node_t *en3= init_entry_node(e3);
    insert_entry(g_entries, en3, after_date);

    mu_assert(g_entries->num_nodes == 3, "Entries", 76);
    mu_assert(g_entries->head == en1, "Entries", 77);
    mu_assert(g_entries->head->next == en2, "Entries", 78);
    mu_assert(g_entries->tail == en3, "Entries", 79);
    mu_assert(g_entries->tail->prev == en2, "Entries", 80);
    mu_assert(en2->next == en3, "Entries", 81);
    mu_assert(en2->prev == en1, "Entries", 82);

    // one day before (should be second now)
    struct tm tm4 = {.tm_sec=0, .tm_min=0, .tm_hour=0,
	   .tm_mday=13, .tm_mon=0, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date4 = mktime(&tm4);
    entry_t *e4 = init_entry(4, "A Name", date4, -12.00, 1, "A Note");
    entry_node_t *en4= init_entry_node(e4);
    insert_entry(g_entries, en4, after_date);

    mu_assert(g_entries->num_nodes == 4, "Entries", 83);
    mu_assert(g_entries->head == en1, "Entries", 84);
    mu_assert(g_entries->head->next == en4, "Entries", 85);
    mu_assert(en4->next == en2, "Entries", 86);
    mu_assert(en4->prev == en1, "Entries", 87);
    mu_assert(en2->prev == en4, "Entries", 88);

    free_entry_list(g_entries);
    return 0;
}


int dist_between_test() {
    int d;
    g_entries = test_dummy_entry_list(1);
    d = dist_between(g_entries->head, g_entries->tail);
    mu_assert(d == 0, "Entries", 89);
    free_entry_list(g_entries);

    g_entries = test_dummy_entry_list(5);
    d = dist_between(g_entries->head, g_entries->tail);
    mu_assert(d == 4, "Entries", 90);
    d = dist_between(g_entries->tail, g_entries->head);
    mu_assert(d == 4, "Entries", 91);
    free_entry_list(g_entries);

    entry_node_t *en1 = init_entry_node(test_dummy_entry(1));
    entry_node_t *en2 = init_entry_node(test_dummy_entry(2));
    d = dist_between(en1, en2);
    mu_assert(d == -1, "Entries", 92);
    free_entry_node(en1);
    free_entry_node(en2);

    return 0;
}
