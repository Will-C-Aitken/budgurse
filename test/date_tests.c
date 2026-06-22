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
    // No date params defaults to one year from today
    date_context_t *dc = init_date_context(0, 0, MONTH);

    time_t cur_time = time(NULL);
    struct tm cur_tm = *localtime(&cur_time);
    struct tm end_tm = *localtime(&dc->end);
    struct tm start_tm = *localtime(&dc->start);

    mu_assert(end_tm.tm_mday == days_in_mnth[cur_tm.tm_mon], "Date", 1);
    mu_assert(end_tm.tm_mon == cur_tm.tm_mon, "Date", 2);
    mu_assert(end_tm.tm_year == cur_tm.tm_year, "Date", 3);

    mu_assert(start_tm.tm_mday == 1, "Date", 4);
    mu_assert(start_tm.tm_mon == (cur_tm.tm_mon + 1) % 12, "Date", 5);
    mu_assert(start_tm.tm_year == cur_tm.tm_year - 1, "Date", 6);

    free_date_context(dc);

    // Just end time defaults to one year before then for start
    struct tm tm1 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=12, 
	.tm_mon=1, .tm_year=2022 - 1900, .tm_isdst=1}; // 02/12/2022
    time_t date1 = mktime(&tm1);
    dc = init_date_context(0, date1, MONTH);
    start_tm = *localtime(&dc->start);
    end_tm = *localtime(&dc->end);

    mu_assert(end_tm.tm_mday == 28, "Date", 7); // last day of feb
    mu_assert(end_tm.tm_mon == 1, "Date", 8); // feb
    mu_assert(end_tm.tm_year == 2022 - 1900, "Date", 9);

    mu_assert(start_tm.tm_mday == 1, "Date", 10);
    mu_assert(start_tm.tm_mon == 2, "Date", 11); // march
    mu_assert(start_tm.tm_year == 2021 - 1900, "Date", 12);

    free_date_context(dc);

    // Just start defaults to one year after for end
    dc = init_date_context(date1, 0, MONTH);
    start_tm = *localtime(&dc->start);
    end_tm = *localtime(&dc->end);

    mu_assert(end_tm.tm_mday == 31, "Date", 13); // last day of jan
    mu_assert(end_tm.tm_mon == 0, "Date", 14); // jan
    mu_assert(end_tm.tm_year == 2023 - 1900, "Date", 15);

    mu_assert(start_tm.tm_mday == 1, "Date", 16);
    mu_assert(start_tm.tm_mon == 1, "Date", 17); // feb
    mu_assert(start_tm.tm_year == 2022 - 1900, "Date", 18); 

    free_date_context(dc);

    // If both date param are supplied, start cannot be g.t. end
    struct tm tm2 = {.tm_sec=0, .tm_min=0, .tm_hour=0, .tm_mday=12, 
	.tm_mon=2, .tm_year=2022 - 1900, .tm_isdst=1}; 
    time_t date2 = mktime(&tm2);
    dc = init_date_context(date2, date1, MONTH);
    mu_assert(!dc, "Date", 19);

    // start == end is legal (covers one whole month)
    dc = init_date_context(date2, date2, MONTH);
    mu_assert(dc, "Date", 20);
    start_tm = *localtime(&dc->start);
    end_tm = *localtime(&dc->end);

    mu_assert(end_tm.tm_mday == 31, "Date", 21); // last day of march
    mu_assert(start_tm.tm_mday == 1, "Date", 22);
    mu_assert(end_tm.tm_mon == start_tm.tm_mon, "Date", 23);
    mu_assert(end_tm.tm_year == start_tm.tm_year, "Date", 24);

    free_date_context(dc);

    return 0;
}
