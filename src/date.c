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
#include "entries.h"
#include "browser.h"

date_context_t *g_date_context = NULL;

const int days_in_mnth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// pass 0, 0 for default start and end
date_context_t *init_date_context(time_t start, time_t end, date_delin_t d) {

    struct tm curr_tm, end_tm, start_tm;
    time_t curr_time = time(NULL);

    date_context_t *dc = malloc(sizeof(date_context_t));
    dc->date_delin = d;
    curr_tm = *localtime(&curr_time);

    // Default end if neither parameter is current day. Default range is 1 year
    if (!end) {
	if (!start) {
	    end_tm = curr_tm;
	} else {
	    start_tm = *localtime(&start);
	    end_tm = start_tm;
	    end_tm.tm_year += 1;
	    end_tm.tm_mon = (end_tm.tm_mon - 1) % 12;
	}
    } else {
	end_tm = *localtime(&end);
    }

    if (!start) {
	start_tm = end_tm;
	start_tm.tm_year -= 1;
	start_tm.tm_mon = (start_tm.tm_mon + 1) % 12;
    } else {
	start_tm = *localtime(&start);
    }


    switch (d) {
	// trim to first day of start month and last day of end month
	case MONTH: 
	    start_tm.tm_mday = 1;
	    end_tm.tm_mday = days_in_mnth[end_tm.tm_mon];
	    break;
	default: 
	    break;
    }

    clean_tm(&start_tm);
    clean_tm(&end_tm);

    dc->start = mktime(&start_tm);
    dc->end = mktime(&end_tm);

    if (dc->start > dc->end) {
	free(dc);
	return NULL;
    }

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
    struct tm tm_from_date = *localtime(&date);
    switch (d) {
       case WEEK: 
           break;
       case MONTH: 
           date_part = tm_from_date.tm_mon;
           break;
       case YEAR: 
           date_part = tm_from_date.tm_year + 1900;
           break;
    }
    return date_part;
}


void update_date_context(date_delin_t d, int amount) {
    struct tm new_start_tm = *localtime(&g_date_context->start);
    struct tm new_end_tm = *localtime(&g_date_context->end);
    time_t new_start, new_end;
    int sel_id = -1;

    if (!amount)
	return;

    date_update_tm(&new_start_tm, d, amount);
    date_update_tm(&new_end_tm, d, amount);

    new_start = mktime(&new_start_tm);
    new_end = mktime(&new_end_tm);

    date_delin_t cur_d = g_date_context->date_delin;
    g_date_context = init_date_context(new_start, new_end, cur_d);

    // reset browser and entry list, preserving sel
    if (g_browser && g_browser->sel)
	sel_id = ((entry_t *)g_browser->sel->data)->id;

    g_browser->sel = NULL;
    free_llist(g_entries, (llist_free_data_fn_t)free_entry);

    g_entries = init_llist();
    load_entry_table(g_date_context, sel_id);

    // g_browser now has garbage data, EXCEPT for sel which is node in new
    // list. Save it
    llist_node_t *new_sel = g_browser->sel;

    free_browser(g_browser);
    // could not find sel in new entry list
    if (!new_sel) {
	if (amount > 0)
	    g_browser = init_browser(g_entries, g_entries->head, 0, -1);
	else
	    g_browser = init_browser(g_entries, g_entries->tail, 0, -1);
    } else
	g_browser = init_browser(g_entries, new_sel, 0, -1);
}


void date_update_tm(struct tm *tm, date_delin_t d, int amount) {

    switch (d) {
       case WEEK: 
           break;
       case MONTH:
           tm->tm_mon = tm->tm_mon + amount;
           if (tm->tm_mon >= 0)
               tm->tm_year += tm->tm_mon/12; 
           else
               tm->tm_year += (tm->tm_mon/12) - 1; 
           tm->tm_mon = (tm->tm_mon + 12) % 12; 
           if (tm->tm_mday > 1)
               tm->tm_mday = days_in_mnth[tm->tm_mon];
           break;
       case YEAR: 
           tm->tm_year += amount;
           break;
    }
}
