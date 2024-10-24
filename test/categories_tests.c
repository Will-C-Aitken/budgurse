#include "test.h"

int categories_tests() {
    mu_run_test(cat_array_test);
    mu_run_test(cat_id_to_names_test);
    return 0;
}


int cat_array_test() {

    int rc;
    // empty category array
    g_categories = init_cat_array();
    mu_assert(!g_categories->array, "Categories", 1);
    mu_assert(!g_categories->num_cats, "Categories", 2);

    // Signature of (parent_id, id, name). 0 is NULL category
    category_t *c1 = init_category(1, 0, "Food");
    mu_assert(c1->id == 1, "Categories", 3);
    mu_assert(c1->parent_id == 0, "Categories", 4);
    mu_assert(strcmp(c1->name, "Food") == 0, "Categories", 5);

    free_category(c1);

    // Parent category is NULL, ie Food will be a main category. legal
    c1 = init_category(1, 0, "Food");
    rc = append_to_cat_array(g_categories, c1);
    mu_assert(g_categories->array[0] == c1, "Categories", 6);
    mu_assert(!rc, "Categories", 7);

    // Parent category is Food, which is already in array. legal
    category_t *c2 = init_category(2, 1, "Cafe");
    append_to_cat_array(g_categories, c2);
    mu_assert(g_categories->array[0] == c1, "Categories", 8);
    mu_assert(g_categories->array[1] == c2, "Categories", 9);
    mu_assert(!rc, "Categories", 10);

    // Parent category does not exist. should fail
    category_t *c3 = init_category(3, 3, "Cafe");
    rc = append_to_cat_array(g_categories, c3);
    mu_assert(rc, "Categories", 11);
    mu_assert(g_categories->num_cats == 2, "Categories", 12);
    free_category(c3);

    // Parent category is subcategory. should fail
    c3 = init_category(3, 2, "Cafe");
    rc = append_to_cat_array(g_categories, c3);
    mu_assert(rc, "Categories", 13);
    mu_assert(g_categories->num_cats == 2, "Categories", 14);
    free_category(c3);

    free_cat_array(g_categories);

    return 0;
}


int cat_id_to_names_test() {

    char *cat, *subcat;

    g_categories = init_cat_array();
    category_t *c1 = init_category(1, 0, "Food");
    append_to_cat_array(g_categories, c1);

    cat_id_to_names(g_categories, 1, &cat, &subcat);
    mu_assert(strcmp(cat, "Food") == 0, "Categories", 15);
    mu_assert(!subcat, "Categories", 16);

    category_t *c2 = init_category(2, 1, "Cafe");
    append_to_cat_array(g_categories, c2);

    // category one is still just food without subcat
    cat_id_to_names(g_categories, 1, &cat, &subcat);
    mu_assert(strcmp(cat, "Food") == 0, "Categories", 17);
    mu_assert(!subcat, "Categories", 18);
    
    // category one is still just food without subcat
    cat_id_to_names(g_categories, 2, &cat, &subcat);
    mu_assert(strcmp(cat, "Food") == 0, "Categories", 19);
    mu_assert(strcmp(subcat, "Cafe") == 0, "Categories", 20);

    free_cat_array(g_categories);
    return 0;
}
