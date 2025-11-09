/* budgurse - budgeting with curses
 *
 * Copyright (c) 2025 W. C. Aitken 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "test.h"

int tests_run = 0;
int curses_mode = 0;
state_t state = BROWSER;

int main(int argc, char **argv) {

    if (
	llist_tests() || 
	entries_tests() ||
	backend_tests() ||
	browser_tests() || 
	categories_tests() ||
	prompt_tests() ||
	summary_tests() ||
	misc_tests()
    ) return 1;

    printf("All Tests Passed. Tests run: %d\n", tests_run);
    return 0;
}


// needed for compiling exit macros
void end_budgurse(int status) {
    
    free_browser(g_browser);
    free_llist(g_categories, (llist_free_data_fn_t)free_category);
    free_llist(g_entries, (llist_free_data_fn_t)free_entry);
    free_wins(g_wins);
    free_summary(g_summary);
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
