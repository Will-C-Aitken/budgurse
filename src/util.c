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

#include "util.h"

const int days_in_mnth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void draw_amount(WINDOW *w, float amount, int max_width, 
	const char *delim_str, int ra, int attrs, uint32_t dec_places) {

    waddstr(w, delim_str);

    char *amnt_str = NULL;
    uint32_t amnt_len = amnt_to_str(amount, &amnt_str, dec_places, max_width);

    // turn off attributes
    if (attrs)
	wattrset(w, attrs);

    // right align
    if (ra) {
        while (max_width - amnt_len++ > 0) 
            waddch(w, ' ');
    }

    if (amnt_str)
	wprintw(w, "%s", amnt_str);

    // turn off attributes
    if (attrs)
	wattrset(w, 0);

    free(amnt_str);
}


void draw_ra_string(WINDOW *w, const char *str, int max_width,
	const char *delim_str, int attrs) {

    waddstr(w, delim_str);

    // to right align
    int sl = strlen(str);
    int num_spaces = (sl > max_width) ? 0 : max_width - sl;
    while (num_spaces-- > 0) 
	waddch(w, ' ');

    wattrset(w, attrs);

    // trunacte string
    if (sl > max_width) {
	char trunc_str[max_width+1];
	snprintf(trunc_str, max_width+1, "%-*s", max_width, str);
	wprintw(w, "%s", trunc_str);
    } else
	wprintw(w, "%s", str);

    wattrset(w, 0);
}


void draw_str(WINDOW *w, const char *str, int max_width, 
	const char *delim_str, int attrs) {

    waddstr(w, delim_str);

    if (attrs)
	wattrset(w, attrs);

    // trunacte string
    char trunc_str[max_width+1];
    snprintf(trunc_str, max_width+1, "%-*s", max_width, str);
    // replace last three char with "..."
    if (strlen(str) > max_width) {
	for (int i = 0; i < 3; i++)
	    trunc_str[max_width - 3 + i] = '.';
    }
    wprintw(w, "%s", trunc_str);

    // turn off attributes
    if (attrs)
	wattrset(w, 0);
}


void draw_date(WINDOW *w, time_t date, int attrs) {

    if (attrs)
	wattrset(w, attrs);

    struct tm *tmp_date = localtime(&date);
    wprintw(w, "%02d/%02d/%04d", ++(tmp_date->tm_mon), tmp_date->tm_mday, 
	tmp_date->tm_year + 1900);

    if (attrs)
	wattrset(w, 0);
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


int date_part_from_delin(time_t date, delin_t d) { 
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


void update_date(time_t *date, delin_t d, int amount) { 
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

int num_places_in_amount(int n) {
    if (n < 0) 
	return num_places_in_amount((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) 
	return 1;
    return 1 + num_places_in_amount(n / 10);
}

// up to 2 dec_places precision
bool float_eq_zero(float amnt, uint32_t dec_places) {
    if (fabsf(amnt) >= 0.5)
	return false;
    else {
	if (dec_places == 0)
	    return true;
	else
	    return float_eq_zero(amnt*10, --dec_places);
    }
}

/* Convert a floating point amnt into a str up to max_len with a specified
 * number of dec_places. The str argument takes a pointer to a string which
 * will get a dynamically allocated string assigned to it. Needs to be freed
 * afterward. If the amount cannot fit within max_len, and is greater than a
 * thousand, it will be abbreviated with a 'K' suffix. e.g. -$123,456.87 would
 * be truncated to -$123.46K. Returns the length of the new str not including
 * the null terminator. i.e. on failure, 0 will be returned.
 */
uint32_t amnt_to_str(float amnt, char **str, uint32_t dec_places,
	uint32_t max_len) {

    uint32_t cur_len = 0;

    // minimum len of amnt str
    if (max_len < 4) {
	*str = NULL;
	return 0;
    }

    // max_len + 1 for null termination
    *str = malloc((max_len++)*sizeof(char));
    if (!*str)
	return cur_len;

    if (amnt > MAX_AMOUNT_VAL || amnt < MIN_AMOUNT_VAL) {
	char *max_min_str = (amnt > MAX_AMOUNT_VAL) ? "MAX " : "MIN ";
	strncpy(*str, max_min_str, strlen(max_min_str) + 1); // +1 for \0
	return strlen(*str);
    }

    // + 1 for '.' 
    uint32_t dec_width = (dec_places) ? dec_places + 1 : 0;
    uint32_t ones_width = 0;
    uint32_t num_dig;
    int num_spaces; 
    char suffix = ' ';
    float thousands;
    num_dig = num_places_in_amount((int)amnt);

    // four extra spaces for possible '$', '-', '[KMB]', and null char 
    num_spaces = max_len - num_dig - dec_width - 4; 
    thousands = fabsf(amnt / 1000.0);
    // another space for ','
    if ((int)thousands > 0)
	--num_spaces;

    // out of space, try converting thousands from comma format to K suffix
    if (num_spaces < 0) {
	// add three dig + comma but only if thousands will round up to at
	// least one, or if there are enough decimal places such that the
	// result won't be 0 when divided by a thousand
	if ((thousands > 0.50 || 
		(dec_places && dec_places < 3 &&
		 roundf(thousands*powf(10, dec_places)) > 0)) &&
		(num_spaces += 4) >= 0) {
	    suffix = 'K';
	} else {
	    *str = NULL;
	    return cur_len;
	}
    }

    if (float_eq_zero(amnt, dec_places)) {
	strncpy(*str, "--- ", 5);
	return strlen(*str);
    }

    **str = (amnt < 0.0) ? '-' : ' ';
    amnt = fabsf(amnt);
    *(*str + 1) = '$';
    cur_len = 2;

    // Short form
    if (suffix == 'K') {
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%.*fK",
		dec_places, thousands);
	return cur_len;
    } 

    // Long form. Print just thousands + ',' first
    if ((int)thousands) {
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%d,",
		(int)thousands);
	amnt = (amnt - 1000*(int)thousands);
	ones_width = 3;
    }

    if (dec_places > 0)
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%0*.*f ",
		ones_width + dec_width, dec_places, amnt);
    else
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%0*d ",
		ones_width + dec_width, (int)(roundf(amnt)));

    return cur_len;
}
