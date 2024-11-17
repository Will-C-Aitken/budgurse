#include "test.h"

int prompt_tests() {
    mu_run_test(date_proc_test);
    mu_run_test(amount_proc_test);
    mu_run_test(cat_proc_test);
    return 0;
}


int date_proc_test() {

    char *test_string = "11/17/2023";
    time_t result;
    mu_assert(!date_proc(test_string, &result), "Prompt", 1);
    
    struct tm *result_tm = localtime(&result);
    mu_assert(result_tm->tm_mday == 17, "Prompt", 2);
    mu_assert(result_tm->tm_mon == 11-1, "Prompt", 3);
    mu_assert(result_tm->tm_year == 2023 - 1900, "Prompt", 4);

    // missing slashes
    test_string = "11172024";
    mu_assert(date_proc(test_string, &result), "Prompt", 5);

    // valid shortform
    test_string = "11/12";
    mu_assert(!date_proc(test_string, &result), "Prompt", 6);
    result_tm = localtime(&result);
    mu_assert(result_tm->tm_mday == 12, "Prompt", 7);
    mu_assert(result_tm->tm_mon == 11-1, "Prompt", 8);
    mu_assert(result_tm->tm_year == 2024 - 1900, "Prompt", 9);

    // invalid month
    test_string = "13/14";
    mu_assert(date_proc(test_string, &result), "Prompt", 10);

    // invalid day
    test_string = "11/31";
    mu_assert(date_proc(test_string, &result), "Prompt", 11);

    return 0;
}


int amount_proc_test() {
    int rc;
    float result;
    char *test_str = "";

    // empty string 
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 12);

    // cannot be 0
    test_str = "0.00";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 13);

    test_str = "not a digit";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 14);

    // legal float + non number
    test_str = "14.00 and not a digit";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 15);
    test_str = "not a digit and 14.00";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 16);

    // commas are not allowed
    test_str = "14,000.00";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 17);
    
    // legal
    test_str = "14000.02";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_SUCCESS, "Prompt", 18);
    mu_assert(is_amount_eq(result, 14000.02), "Prompt", 19);

    // legal -ve
    test_str = "-12";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_SUCCESS, "Prompt", 20);
    mu_assert(is_amount_eq(result, -12.00), "Prompt", 21);

    // too large and small
    test_str = "1000000";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 22);
    test_str = "-100000";
    rc = amount_proc(test_str, &result);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 23);

    return 0;
}

int cat_proc_test() {

    g_categories = init_llist();
    
    int rc;
    int id;
    char *test_str = "";

    // can't be empty
    rc = m_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 24);

    test_str = "Food";
    category_t *c1 = init_category(1, 0, test_str);
    llist_insert_node(g_categories, init_llist_node(c1), 
	    (llist_comp_fn_t)cat_comp);
    rc = m_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_SUCCESS, "Prompt", 25);

    test_str = "Max characterssssssssssssssssss";
    category_t *c2 = init_category(2, 0, test_str);
    llist_insert_node(g_categories, init_llist_node(c2), 
	    (llist_comp_fn_t)cat_comp);
    rc = m_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_SUCCESS, "Prompt", 26);
    mu_assert(id == 2, "Prompt", 27);

    test_str = "Too many characterssssssssssssss";
    category_t *c3 = init_category(3, 0, test_str);
    llist_insert_node(g_categories, init_llist_node(c3), 
	    (llist_comp_fn_t)cat_comp);
    rc = m_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 28);

    // Can't be a subcategory
    test_str = "Cafe";
    category_t *c4 = init_category(4, 1, test_str);
    llist_insert_node(g_categories, init_llist_node(c4), 
	    (llist_comp_fn_t)cat_comp);
    rc = m_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 29);

    // SUBCAT TESTS
    // Can't be main category
    test_str = "Food";
    rc = s_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 30);

    // legal
    test_str = "Max characterssssssssssssssssss";
    // make it a subcat of Food
    category_t *c5 = init_category(5, 1, test_str);
    llist_insert_node(g_categories, init_llist_node(c5), 
	    (llist_comp_fn_t)cat_comp);
    // id should be set to parent_id before entering fn
    id = 1;
    rc = s_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_SUCCESS, "Prompt", 31);
    mu_assert(id == 5, "Prompt", 32);

    // legal except too long
    test_str = "Too many characterssssssssssssss";
    category_t *c6 = init_category(6, 0, test_str);
    llist_insert_node(g_categories, init_llist_node(c6), 
	    (llist_comp_fn_t)cat_comp);
    rc = s_cat_proc(test_str, &id);
    mu_assert(rc == BUDGURSE_FAILURE, "Prompt", 33);

    // Subcat CAN be empty. id should be kept as is
    test_str = "";
    rc = s_cat_proc(test_str, &id);
    id = 1;
    mu_assert(rc == BUDGURSE_SUCCESS, "Prompt", 34);
    mu_assert(id == 1, "Prompt", 35);

    free_llist(g_categories, (llist_free_data_fn_t)free_category);

    return 0;
}

