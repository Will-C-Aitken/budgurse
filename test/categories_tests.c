#include "test.h"

int categories_tests() {
    mu_run_test(init_categories_test);
    mu_run_test(cat_id_to_names_test);
    mu_run_test(cat_name_to_id_test);
    mu_run_test(cat_is_sub_test);
    mu_run_test(cat_set_sum_idxs_test);
    mu_run_test(cat_llist_to_array_test);
    mu_run_test(cat_get_from_name_test);
    mu_run_test(cat_del_from_llist_test);
    return 0;
}


int init_categories_test() {

    g_categories = init_llist();

    category_t *c1 = init_category(1, 0, "Food");
    mu_assert(c1->id == 1, "Categories", 1);
    mu_assert(c1->p_id == 0, "Categories", 2);
    mu_assert(strcmp(c1->name, "Food") == 0, "Categories", 3);
    mu_assert(!c1->subcats, "Categories", 4);

    llist_node_t *cn1 = init_llist_node(c1);
    llist_insert_node(g_categories, cn1, (llist_comp_fn_t)cat_comp);

    mu_assert(g_categories->head == cn1, "Categories", 3);
    mu_assert(g_categories->tail == cn1, "Categories", 4);
    mu_assert(g_categories->num_nodes == 1, "Categories", 5);
    mu_assert(!c1->subcats, "Categories", 6);

    // subcateogry of prev cat
    category_t *c2 = init_category(2, 1, "Cafe");
    llist_node_t *cn2 = init_llist_node(c2);
    llist_insert_node(g_categories, cn2, (llist_comp_fn_t)cat_comp);
    mu_assert(c1->subcats->num_nodes == 1, "Categories", 7);
    mu_assert(c1->subcats->head == cn2, "Categories", 8);
    mu_assert(c1->subcats->tail == cn2, "Categories", 9);
    mu_assert(g_categories->num_nodes == 1, "Categories", 10);

    // subcateogory with higher name
    category_t *c3 = init_category(3, 1, "Banana");
    llist_node_t *cn3 = init_llist_node(c3);
    llist_insert_node(g_categories, cn3, (llist_comp_fn_t)cat_comp);
    mu_assert(c1->subcats->num_nodes == 2, "Categories", 11);
    mu_assert(c1->subcats->head == cn3, "Categories", 12);
    mu_assert(c1->subcats->tail == cn2, "Categories", 13);
    mu_assert(g_categories->num_nodes == 1, "Categories", 14);

    // main category with higher name
    category_t *c4 = init_category(4, 0, "Apparel");
    llist_node_t *cn4 = init_llist_node(c4);
    llist_insert_node(g_categories, cn4, (llist_comp_fn_t)cat_comp);
    mu_assert(c1->subcats->num_nodes == 2, "Categories", 15);
    mu_assert(c1->subcats->head == cn3, "Categories", 16);
    mu_assert(c1->subcats->tail == cn2, "Categories", 17);
    mu_assert(g_categories->num_nodes == 2, "Categories", 18);
    mu_assert(g_categories->head == cn4, "Categories", 19);
    mu_assert(g_categories->tail == cn1, "Categories", 20);

    // lower case should be bottom
    category_t *c5 = init_category(5, 0, "apparel");
    llist_node_t *cn5 = init_llist_node(c5);
    llist_insert_node(g_categories, cn5, (llist_comp_fn_t)cat_comp);
    // now 
    // cn4 
    //  |
    //  v
    // cn1 -> (cn3 -> cn2)
    //  |
    //  v
    // cn5
    mu_assert(g_categories->num_nodes == 3, "Categories", 21);
    mu_assert(g_categories->head == cn4, "Categories", 22);
    mu_assert(g_categories->tail == cn5, "Categories", 23);

    // subcat of new bottom
    category_t *c6 = init_category(6, 5, "Shoes");
    llist_node_t *cn6 = init_llist_node(c6);
    llist_insert_node(g_categories, cn6, (llist_comp_fn_t)cat_comp);
    // now 
    // cn4 
    //  |
    //  v
    // cn1 -> (cn3 -> cn2)
    //  |
    //  v
    // cn5 -> (cn6)
    
    // c1 shouldn't change
    mu_assert(c1->subcats->num_nodes == 2, "Categories", 24);
    mu_assert(c1->subcats->head == cn3, "Categories", 25);
    mu_assert(c1->subcats->tail == cn2, "Categories", 26);
    mu_assert(g_categories->num_nodes == 3, "Categories", 27);
    mu_assert(g_categories->head == cn4, "Categories", 28);
    mu_assert(g_categories->tail == cn5, "Categories", 29);
    mu_assert(c5->subcats->num_nodes == 1, "Categories", 30);
    mu_assert(c5->subcats->head == cn6, "Categories", 31);
    mu_assert(c5->subcats->tail == cn6, "Categories", 32);

    // can't make subcategory of subcategory
    category_t *c7 = init_category(7, 6, "Shoes");
    llist_node_t *cn7 = init_llist_node(c7);
    int r = llist_insert_node(g_categories, cn7, (llist_comp_fn_t)cat_comp);
    mu_assert(!r, "Categories", 33);
    // everything should stay the same
    mu_assert(c1->subcats->num_nodes == 2, "Categories", 34);
    mu_assert(c1->subcats->head == cn3, "Categories", 35);
    mu_assert(c1->subcats->tail == cn2, "Categories", 36);
    mu_assert(g_categories->num_nodes == 3, "Categories", 37);
    mu_assert(g_categories->head == cn4, "Categories", 38);
    mu_assert(g_categories->tail == cn5, "Categories", 39);
    mu_assert(c5->subcats->num_nodes == 1, "Categories", 40);
    mu_assert(c5->subcats->head == cn6, "Categories", 41);
    mu_assert(c5->subcats->tail == cn6, "Categories", 42);
    free_llist_node(cn7, (llist_free_data_fn_t)free_category);

    // inserting subcategory at head (same name as other category which is
    // allowed)
    category_t *c8 = init_category(8, 4, "apparel");
    llist_node_t *cn8 = init_llist_node(c8);
    llist_insert_node(g_categories, cn8, (llist_comp_fn_t)cat_comp);
    // now 
    // cn4 -> (cn8)
    //  |
    //  v
    // cn1 -> (cn3 -> cn2)
    //  |
    //  v
    // cn5 -> (cn6)
    mu_assert(c1->subcats->num_nodes == 2, "Categories", 43);
    mu_assert(c1->subcats->head == cn3, "Categories", 44);
    mu_assert(c1->subcats->tail == cn2, "Categories", 45);
    mu_assert(g_categories->num_nodes == 3, "Categories", 46);
    mu_assert(g_categories->head == cn4, "Categories", 47);
    mu_assert(g_categories->tail == cn5, "Categories", 48);
    mu_assert(c5->subcats->num_nodes == 1, "Categories", 49);
    mu_assert(c5->subcats->head == cn6, "Categories", 50);
    mu_assert(c5->subcats->tail == cn6, "Categories", 51);
    mu_assert(c4->subcats->num_nodes == 1, "Categories", 52);
    mu_assert(c4->subcats->head == cn8, "Categories", 53);
    mu_assert(c4->subcats->tail == cn8, "Categories", 54);

    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    return 0;
}

int cat_id_to_names_test() {
    char *main_name, *subname;
    llist_t* tcl = test_dummy_cat_list(5);

    cat_id_to_names(tcl, 3, &main_name, &subname);
    mu_assert(strcmp(main_name, "Cat3") == 0, "Categories", 55);
    mu_assert(!subname, "Categories", 56);

    cat_id_to_names(tcl, 4, &main_name, &subname);
    mu_assert(strcmp(main_name, "Cat3") == 0, "Categories", 57);
    mu_assert(strcmp(subname, "Cat4") == 0, "Categories", 58);

    free_llist(tcl, (llist_free_data_fn_t)free_category);
    return 0;
}

int cat_name_to_id_test() {
    int id;
    llist_t* tcl = test_dummy_cat_list(5);

    id = cat_name_to_id(tcl, "Cat5", 0);
    mu_assert(id == 5, "Categories", 59);
    id = cat_name_to_id(tcl, "Cat4", 3);
    mu_assert(id == 4, "Categories", 60);
    // cat 4 exists but not as a main category. should fail
    id = cat_name_to_id(tcl, "Cat4", 0);
    mu_assert(id == 0, "Categories", 61);
    // cat 4 exists but not as a subcategory of id 1
    id = cat_name_to_id(tcl, "Cat4", 1);
    mu_assert(id == 0, "Categories", 62);

    free_llist(tcl, (llist_free_data_fn_t)free_category);
    return 0;
}

int cat_is_sub_test() {

    llist_t* tcl = test_dummy_cat_list(5);

    mu_assert(cat_is_sub(tcl, 2, 1), "Categories", 63);
    mu_assert(!cat_is_sub(tcl, 3, 1), "Categories", 64);
    mu_assert(!cat_is_sub(tcl, 4, 1), "Categories", 65);

    free_llist(tcl, (llist_free_data_fn_t)free_category);
    return 0;
}

int cat_set_sum_idxs_test() {
    int next_id = 0;

    llist_t* tcl = init_llist();
    cat_set_sum_idxs(tcl, &next_id);
    mu_assert(next_id == 0, "Categories", 66);
    free_llist(tcl, (llist_free_data_fn_t)free_category);

    tcl = test_dummy_cat_list(5);
    cat_set_sum_idxs(tcl, &next_id);
    mu_assert(next_id == 5, "Categories", 67);
    mu_assert(((category_t *)tcl->head->data)->sum_idx == 0 , "Categories", 68);
    mu_assert(((category_t *)tcl->tail->data)->sum_idx == 4 , "Categories", 69);

    // insert node and update idxs
    category_t *c6 = init_category(6, 0, "Cat21");
    llist_node_t *cn6 = init_llist_node(c6);
    llist_insert_node(tcl, cn6, (llist_comp_fn_t)cat_comp);
    next_id = 0;
    cat_set_sum_idxs(tcl, &next_id);
    mu_assert(((category_t *)tcl->head->data)->sum_idx == 0 , "Categories", 70);
    mu_assert(c6->sum_idx == 2, "Categories", 71);
    mu_assert(((category_t *)tcl->tail->data)->sum_idx == 5 , "Categories", 72);
    free_llist(tcl, (llist_free_data_fn_t)free_category);

    return 0;
}

int cat_llist_to_array_test() {

    int temp_idx = 0;
    category_t **ca = malloc(sizeof(category_t *) * 5);
    llist_t *tcl = test_dummy_cat_list(5);
    cat_llist_to_array(tcl, &ca, &temp_idx);
    mu_assert(strcmp(ca[0]->name, "Cat1") == 0 , "Categories", 73);
    mu_assert(strcmp(ca[3]->name, "Cat4") == 0 , "Categories", 74);
    mu_assert(strcmp(ca[4]->name, "Cat5") == 0 , "Categories", 75);
    free(ca);
    free_llist(tcl, (llist_free_data_fn_t)free_category);

    return 0;
}

int cat_get_from_name_test() {
    llist_t *tcl = test_dummy_cat_list(5);
    category_t *c = cat_get_from_name(tcl, "Cat4");
    mu_assert(c->id == 4, "Categories", 76);
    c = cat_get_from_name(tcl, "Cat1000");
    mu_assert(!c, "Categories", 77);
    free_llist(tcl, (llist_free_data_fn_t)free_category);
    return 0;
}


int cat_del_from_llist_test() {
    int rc;
    llist_t *tcl = test_dummy_cat_list(5);
    category_t *c = cat_get_from_name(tcl, "Cat1");

    rc = cat_del_from_llist(tcl, c);
    mu_assert(rc == 1, "Categories", 78);
    mu_assert(tcl->num_nodes == 2, "Categories", 79);

    c = cat_get_from_name(tcl, "Cat5");
    rc = cat_del_from_llist(tcl, c);
    mu_assert(rc == 1, "Categories", 80);
    mu_assert(tcl->num_nodes == 1, "Categories", 81);

    c = cat_get_from_name(tcl, "Cat4");
    rc = cat_del_from_llist(tcl, c);
    mu_assert(rc == 1, "Categories", 82);
    mu_assert(tcl->num_nodes == 1, "Categories", 83);
    mu_assert(!((category_t *)tcl->head->data)->subcats, "Categories", 84);

    c = cat_get_from_name(tcl, "Cat3");
    rc = cat_del_from_llist(tcl, c);
    mu_assert(rc == 1, "Categories", 85);
    mu_assert(tcl->num_nodes == 0, "Categories", 86);

    free_llist(tcl, (llist_free_data_fn_t)free_category);
    return 0;
}
