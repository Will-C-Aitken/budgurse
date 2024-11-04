#include "test.h"

int llist_tests() {
    mu_run_test(append_to_tail_test);
    mu_run_test(del_tail_test);
    mu_run_test(free_entry_list_test);
    mu_run_test(is_end_node_test);
    mu_run_test(del_head_test);
    mu_run_test(del_entry_test);
    mu_run_test(dist_between_test);
    return 0;
}

int append_to_tail_test() {

    llist_t *tl = init_llist();
    llist_node_t *tnd = init_llist_node(NULL);
    llist_insert_node(tl, tnd, llist_to_tail, NULL);

    // there should be no next or prev
    mu_assert(!tl->head->prev, "Llist", 1);
    mu_assert(!tl->head->next, "Llist", 2);
    mu_assert(!tl->tail->next, "Llist", 3);
    mu_assert(!tl->tail->prev, "Llist", 4);
    mu_assert(tl->num_nodes == 1, "Llist", 5);

    // head and tail are same
    mu_assert(tl->tail == tnd, "Llist", 6);
    mu_assert(tl->head == tnd, "Llist", 7);

    llist_node_t* tnd2 = init_llist_node(NULL);
    llist_insert_node(tl, tnd2, llist_to_tail, NULL);

    mu_assert(!tl->head->prev, "Llist", 8);
    mu_assert(tl->head->next == tl->tail, "Llist", 9);
    mu_assert(!tl->tail->next, "Llist", 10);
    mu_assert(tl->tail->prev == tl->head, "Llist", 11);

    mu_assert(tl->head == tnd, "Llist", 12);
    mu_assert(tl->tail == tnd2, "Llist", 13);

    llist_node_t* tnd3 = init_llist_node(NULL);
    llist_insert_node(tl, tnd3, llist_to_tail, NULL);

    mu_assert(!tl->head->prev, "Llist", 14);
    mu_assert(tl->head->next == tnd2, "Llist", 15);
    mu_assert(!tl->tail->next, "Llist", 16);
    mu_assert(tl->tail->prev == tnd2, "Llist", 17);

    mu_assert(tnd2->next == tnd3, "Llist", 18);
    mu_assert(tnd2->prev == tnd, "Llist", 19);

    free_llist(tl, free);

    return 0;
}
 

int del_tail_test() {

    // Create list of 2 entries
    llist_t *tl = test_dummy_list(2);

    // sec, min, hour, day, month, year, is_dst
    llist_del_tail(tl, free);
    mu_assert(tl->num_nodes == 1, "Llist", 20);
    mu_assert(!tl->tail->next, "Llist", 21);
    mu_assert(!tl->tail->prev, "Llist", 22);
    mu_assert(!tl->head->next, "Llist", 23);
    mu_assert(!tl->head->prev, "Llist", 24);
    mu_assert(tl->head == tl->tail, "Llist", 25);

    llist_del_tail(tl, free);
    mu_assert(tl->num_nodes == 0, "Llist", 26);
    mu_assert(!tl->head, "Llist", 27);
    mu_assert(!tl->tail, "Llist", 28);

    llist_del_tail(tl, free);
    free(tl);
    return 0;
}


int free_entry_list_test() {
    llist_t *tl = test_dummy_list(2);
    free_llist(tl, free);
    return 0;
}


int is_end_node_test() {

    llist_t *tl = init_llist();
    llist_node_t* nd1 = init_llist_node(NULL);
    llist_insert_node(tl, nd1, llist_to_tail, NULL);

    mu_assert(llist_is_end_node(nd1), "Llist", 34);

    llist_node_t* nd2 = init_llist_node(NULL);
    llist_insert_node(tl, nd2, llist_to_tail, NULL);

    // Both are still end nodes
    mu_assert(llist_is_end_node(nd2), "Llist", 35);
    mu_assert(llist_is_end_node(nd1), "Llist", 36);

    llist_node_t* nd3 = init_llist_node(NULL);
    llist_insert_node(tl, nd3, llist_to_tail, NULL);

    // 1 and 3 are end nodes but 2 isn't
    mu_assert(llist_is_end_node(nd1), "Llist", 37);
    mu_assert(llist_is_end_node(nd3), "Llist", 38);
    mu_assert(!llist_is_end_node(nd2), "Llist", 39);
    mu_assert(llist_is_head(nd1), "Llist", 40);
    mu_assert(llist_is_tail(nd3), "Llist", 41);

    free_llist(tl, free);
    return 0;
}


int llist_node_traverse_test() {
    llist_t *tl = init_llist();

    llist_node_t* nd1 = init_llist_node(NULL);
    llist_insert_node(tl, nd1, llist_to_tail, NULL);

    llist_node_t* nd2 = init_llist_node(NULL);
    llist_insert_node(tl, nd2, llist_to_tail, NULL);

    llist_node_t* nd3 = init_llist_node(NULL);
    llist_insert_node(tl, nd3, llist_to_tail, NULL);

    llist_node_t* temp = tl->tail;

    // temp should stay at node 3
    llist_node_traverse(&temp, DOWN);
    mu_assert(temp == nd3, "Llist", 42);

    llist_node_traverse(&temp, UP);
    mu_assert(temp == nd2, "Llist", 43);

    llist_node_traverse(&temp, UP);
    mu_assert(temp == nd1, "Llist", 44);

    // temp should stay at node 1
    llist_node_traverse(&temp, UP);
    mu_assert(temp == nd1, "Llist", 45);

    free_llist(tl, free);
    return 0;
}


int del_head_test() {

    llist_t *tl = init_llist();
    llist_node_t* nd1 = init_llist_node(NULL);
    llist_insert_node(tl, nd1, llist_to_tail, NULL);

    llist_node_t* nd2 = init_llist_node(NULL);
    llist_insert_node(tl, nd2, llist_to_tail, NULL);
    
    llist_node_t* nd3 = init_llist_node(NULL);
    llist_insert_node(tl, nd3, llist_to_tail, NULL);

    llist_del_head(tl, free);
    mu_assert(nd2 == tl->head, "Llist", 46);
    mu_assert(nd2->prev == NULL, "Llist", 47);
    mu_assert(tl->num_nodes == 2, "Llist", 48);

    llist_del_head(tl, free);
    mu_assert(tl->head == nd3, "Llist", 49);
    mu_assert(tl->tail == nd3, "Llist", 50);
    mu_assert(nd3->prev == NULL, "Llist", 51);
    mu_assert(tl->num_nodes == 1, "Llist", 52);

    llist_del_head(tl, free);
    mu_assert(tl->head == NULL, "Llist", 53);
    mu_assert(tl->tail == NULL, "Llist", 54);
    mu_assert(tl->num_nodes == 0, "Llist", 55);

    free_llist(tl, free);
    return 0;
}


int del_entry_test() {

    llist_t *tl = init_llist();
    llist_node_t* nd1 = init_llist_node(NULL);
    llist_insert_node(tl, nd1, llist_to_tail, NULL);

    llist_node_t* nd2 = init_llist_node(NULL);
    llist_insert_node(tl, nd2, llist_to_tail, NULL);
    
    llist_node_t* nd3 = init_llist_node(NULL);
    llist_insert_node(tl, nd3, llist_to_tail, NULL);

    llist_del_node(tl, nd2, free);
    mu_assert(nd1->next == nd3, "Llist", 57);
    mu_assert(nd3->prev == nd1, "Llist", 58);
    mu_assert(tl->num_nodes == 2, "Llist", 59);

    llist_del_node(tl, nd3, free);
    mu_assert(tl->head == nd1, "Llist", 60);
    mu_assert(tl->tail == nd1, "Llist", 61);
    mu_assert(nd1->next == NULL, "Llist", 62);
    mu_assert(nd1->prev == NULL, "Llist", 63);
    mu_assert(tl->num_nodes == 1, "Llist", 64);

    llist_del_node(tl, nd1, free);
    mu_assert(tl->head == NULL, "Llist", 65);
    mu_assert(tl->tail == NULL, "Llist", 66);
    mu_assert(tl->num_nodes == 0, "Llist", 67);

    free_llist(tl, free);
    return 0;
}


int dist_between_test() {
    int d;
    llist_t *tl = test_dummy_list(1);
    d = llist_dist_between(tl->head, tl->tail);
    mu_assert(d == 0, "Llist", 68);
    free_llist(tl, free);

    tl = test_dummy_list(5);
    d = llist_dist_between(tl->head, tl->tail);
    mu_assert(d == 4, "Llist", 69);
    d = llist_dist_between(tl->tail, tl->head);
    mu_assert(d == 4, "Llist", 70);
    free_llist(tl, free);

    llist_node_t *nd1 = init_llist_node(NULL);
    llist_node_t *nd2 = init_llist_node(NULL);
    d = llist_dist_between(nd1, nd2);
    mu_assert(d == -1, "Llist", 71);
    free_llist_node(nd1, free);
    free_llist_node(nd2, free);

    return 0;
}
