#include <stdio.h>
#include "test.h"
#include "entries.h"

int tests_run = 0;

char* test_append_to_tail() {
    entry_list_t* test_list = init_entry_list();

    entry_t* test_entry1 = init_entry("Starbucks", -12.00, "Food", 
	    "Coffee Shop", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    entry_t* test_entry2 = get_tail(test_list);
    mu_assert("Failure", strcmp(test_entry2->name, "Starbucks") == 0);

    return 0;
}

static char* all_tests() {
    mu_run_test(test_append_to_tail);
    return 0;
}

int main(int argc, char **argv) {
     char *result = all_tests();

     if (result != 0)
         printf("%s\n", result);
     else
         printf("ALL TESTS PASSED\n");

     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
}

