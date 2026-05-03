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

#ifndef DATE_H
#define DATE_H

#include "global.h"

extern const int days_in_mnth[12]; 

typedef enum date_delin {
    WEEK,
    MONTH,
    YEAR,
} date_delin_t;

typedef struct date_context {
    time_t start;
    time_t end;
    bool is_abs_start;
    bool is_abs_end;
    date_delin_t date_delin;
} date_context_t;

date_context_t *init_date_context(time_t start, time_t end, date_delin_t d);
void free_date_context(date_context_t *dc);

// date time functions
int check_time_bounds(int day, int month, int year);
void clean_tm(struct tm *tm_to_clean);

int date_part_from_date_delin(time_t date, date_delin_t d);
void update_date(time_t *date, date_delin_t d, int amount);

#endif
