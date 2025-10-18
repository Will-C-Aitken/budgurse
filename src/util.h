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

#ifndef UTIL_H
#define UTIL_H

#include "status.h"
#include "global.h"

extern const int days_in_mnth[12]; 

// generic drawing functions
void draw_amount(WINDOW *w, float amount, int max_width, 
	const char *delim_str, int ra, int attrs);
void draw_ra_string(WINDOW *w, const char *str, int max_width,
	const char *delim_str, int attrs);
void draw_str(WINDOW *w, const char *str, int max_width, 
	const char *delim_str, int attrs);
void draw_date(WINDOW *w, time_t date, int attrs);

// date time functions
int check_time_bounds(int day, int month, int year);
void clean_tm(struct tm *tm_to_clean);
int date_part_from_delin(time_t date, delin_t d);
void update_date(time_t *date, delin_t d, int amount);

// miscellaneous
int amnt_to_abrv_str(float amnt, char **str);
int num_places_in_amount(int n);
bool make_space_in_amnt_str(int *num_spaces, uint32_t *dec_places);
bool float_eq_zero(float amnt, uint32_t dec_places);
int amnt_to_str(float amnt, char **str, uint32_t ideal_dec_places, 
	uint32_t max_len);

#endif
