#include "test.h"

int tests_run = 0;

entry_list_t *g_entries;
cat_array_t *g_categories;
sqlite3 *g_db;
browser_t *g_browser;
win_t wins[NUM_WINS];

int curses_mode = 0;

int main(int argc, char **argv) {

     if (!entries_tests())
	 printf("All Entries Tests Passed\n");
     if (!backend_tests())
	 printf("All Backend Tests Passed\n");
     if (!browser_tests())
	 printf("All Browser Tests Passed\n");
     if (!categories_tests())
	 printf("All Categories Tests Passed\n");
     if (!prompt_tests())
	 printf("All Prompt Tests Passed\n");

     printf("Tests run: %d\n", tests_run);
}


// needed for compiling exit macros
void end_budgurse(int status) {
    
    free_browser(g_browser);
    free_cat_array(g_categories);
    free_entry_list(g_entries);
    free_wins(g_wins);
    if (sqlite3_close(g_db))
	ERROR_MSG("Failed to properly close database with error message: %s\n",
		  sqlite3_errmsg(g_db));

    // stop ncurses
    endwin();
    exit(status);
}


void print_err_str(const char *test_suite, const char *test_fn_name, 
	int test_num) {
    char err_str[128];
    snprintf(err_str, 128, "Failure in %s Tests, %s: %d", 
	    test_suite, test_fn_name, test_num);
    printf("%s\n", err_str);
}
