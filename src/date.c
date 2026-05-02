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

#include "date.h"

const int days_in_mnth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// pass 0, 0 for default start and end
date_context_t *init_date_context(time_t start, time_t end, date_delin_t d) {

    date_context_t *dc = malloc(sizeof(date_context_t));

    dc->date_delin = d;

    // default to current date
    if (!end)
	end = time(NULL);

    struct tm end_tm = *localtime(&end);
    clean_tm(&end_tm);
    dc->end = mktime(&end_tm);

    struct tm start_tm;

    // default to 12 months
    if (!start) {
	start_tm = end_tm;
	start_tm.tm_year -= 1;
	start_tm.tm_mon -= 1;
    } else
	start_tm = *localtime(&start);

    clean_tm(&start_tm);

    switch (d) {
	// trim to first day of start month
	case MONTH: 
	    start_tm.tm_mday = 1;
	    break;
	default: 
	    break;
    }

    dc->start = mktime(&start_tm);

    // TODO write functions for
    dc->is_abs_start = true;
    dc->is_abs_end = true;

    return dc;
}

void free_date_context(date_context_t *dc) {
    free(dc);
}

int check_time_bounds(int day, int month, int year) {
    // If system has 32 bit time_t, year must be less than 2038
    if (TIME_T_32) 
       if (year < 1900 || year > 2037) return 1;
    if (month < 1 || month > 12) 
       return 1;
    if (day < 0 || day > days_in_mnth[month-1])
       return 1;
    return 0;
}


// set unused date attributes (sec, min, hour, isdst) to 0
void clean_tm(struct tm *tm_to_clean) {
    tm_to_clean->tm_sec = 0;
    tm_to_clean->tm_min = 0;
    tm_to_clean->tm_hour = 0;
    tm_to_clean->tm_isdst = 0;
}


int date_part_from_date_delin(time_t date, date_delin_t d) { 
    int date_part = -1;
    struct tm *tm_from_date = localtime(&date);
    switch (d) {
       case WEEK: 
           break;
       case MONTH: 
           date_part = tm_from_date->tm_mon;
           break;
       case YEAR: 
           date_part = tm_from_date->tm_year + 1900;
           break;
    }
    return date_part;
}


void update_date(time_t *date, date_delin_t d, int amount) { 
    struct tm *tm_from_date = localtime(date);

    switch (d) {
       case WEEK: 
           break;
       case MONTH:
           tm_from_date->tm_mon = tm_from_date->tm_mon + amount;
           if (tm_from_date->tm_mon >= 0)
               tm_from_date->tm_year += tm_from_date->tm_mon/12; 
           else
               tm_from_date->tm_year += (tm_from_date->tm_mon/12) - 1; 
           tm_from_date->tm_mon = (tm_from_date->tm_mon + 12) % 12; 
           if (tm_from_date->tm_mday > 1)
               tm_from_date->tm_mday = days_in_mnth[tm_from_date->tm_mon];
           break;
       case YEAR: 
           tm_from_date->tm_year += amount;
           break;
    }

    *date = mktime(tm_from_date);
}
