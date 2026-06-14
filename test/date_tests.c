/* budgurse - budgeting with curses
 *
 * Copyright (c) 2025-2026 W. C. Aitken 
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

int date_tests() {
    mu_run_test(init_date_context_test);
    return 0;
}

int init_date_context_test() {
    date_context_t *dc = init_date_context(0, 0, MONTH);
    time_t cur_time = time(NULL);
    struct tm cur_tm = *localtime(&cur_time);
    struct tm end_tm = *localtime(&dc->end);
    struct tm start_tm = *localtime(&dc->start);

    mu_assert(end_tm.tm_mday == cur_tm.tm_mday, "Date", 1);
    mu_assert(end_tm.tm_mon == cur_tm.tm_mon, "Date", 2);
    mu_assert(end_tm.tm_year == cur_tm.tm_year, "Date", 3);

    mu_assert(start_tm.tm_mday == 1, "Date", 4);
    mu_assert(start_tm.tm_mon == (end_tm.tm_mon - 1) % 12, "Date", 5);
    mu_assert(start_tm.tm_year == cur_tm.tm_year - 1, "Date", 6);

    free_date_context(dc);

    return 0;
}
