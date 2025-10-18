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
	const char *delim_str, int ra, int attrs) {
    waddstr(w, delim_str);

    // to right align
    int num_spaces, num_dig, thousands;
    num_dig = num_places_in_amount((int)amount);
    num_spaces = max_width - 5 - num_dig;
    thousands = abs((int)amount / 1000);
 

    // for comma
    if (thousands) 
	num_spaces--;

    // right align
    if (ra) {
	while (num_spaces-- > 0) 
	    waddch(w, ' ');
    }

    // print negative (if necessary) and dollar sign 
    if (amount < 0) {
	if (attrs)
	    wattrset(w, attrs);
	waddch(w, '-');
    } else if (ra)
	waddch(w, ' ');
    if (attrs)
	wattrset(w, attrs);
    waddch(w, '$');

    amount = fabs(amount);

    // print comma in thousands
    if (thousands) 
	wprintw(w, "%d,%03.2f", thousands, 
		amount - (thousands*1000));
    else
	wprintw(w, "%0.2f", amount);

    // turn off attributes
    if (attrs)
	wattrset(w, 0);
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


/*  Print the float dollar amount, `amnt` to the string, `str`. Default format
 *  is "[-]$...x,xxx,xxx.xx". If that format would be greater than `max_len`,
 *  try format of [-]$Kxxx.xx with the suffix corresponding to what has
 *  been truncated. e.g. -$123,456,789 would be truncated to -$K123.46. 
 *  If truncated form is still greater than `max_len`, returns 0 and 
 *  does not write to `str`. Returns 1 on success
 *
 *  Somewhat conflicted on what to do about amount. Floats have ~7.2 decimal
 *  digits worth of precision. But there isn't much room to display beyond that
 *  anyway. Hence [K]. But if the values grow far beyond that you'll lose
 *  information. But if you're making/spending millions does the lower
 *  precision even matter? nor would a millionaire need to budget like this.
 *  And if making a corporate budget, likely have to use the tools they provide
 *  and would want more sophistication.
 *
 *  returns length of new string
 */
int amnt_to_abrv_str(float amnt, char **str) {

    int k = 0;
    int neg = (amnt <= -0.5) ? 1 : 0;
    char dig_str[4];

    if (amnt > MAX_AMOUNT_VAL || amnt < MIN_AMOUNT_VAL) {
	*str = NULL;
	return 0;
    }

    *str = malloc((AMOUNT_STR_LEN+1)*sizeof(char));

    amnt = fabsf(amnt); 
    if (amnt > 999.49) {
	k = 1;
	amnt /= 1000.0;
    }
    amnt = roundf(amnt);

    int i = 0;
    if (neg)
	*str[i++] = '-';
    (*str)[i++] = '$';

    // convert to str 
    snprintf(dig_str, 4, "%.f", amnt);

    char *temp = dig_str;
    while (*temp != '\0') {
	(*str)[i++] = *temp;
	temp++;
    } 

    if (k)
	(*str)[i++] = 'K';
    (*str)[i] = '\0';

    return i;
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

bool make_space_in_amnt_str(int *num_spaces, uint32_t *dec_places) {
    // already have space
    if (*num_spaces >= 0)
	return true;

    while (*dec_places > 0) {
	// for final decimal
	*dec_places == 1 ? (*num_spaces) += 2 : (*num_spaces)++;
	(*dec_places)--;
	if (*num_spaces >= 0)
	    return true;
    } 
    return false;
}

int amnt_to_str(float amnt, char **str, uint32_t dec_places, 
	uint32_t max_len) {

    int cur_len = -1;

    if (amnt > MAX_AMOUNT_VAL || amnt < MIN_AMOUNT_VAL) {
	*str = NULL;
	return cur_len;
    }

    char suffix = ' ';
    int num_spaces, num_dig;
    float thousands;
    num_dig = num_places_in_amount((int)amnt);
    printf("num_dig: %d\n", num_dig);

    // three extra spaces for possible '$', '-', and '[KMB]' 
    num_spaces = max_len - num_dig - 3; 
    // + 1 for dec itself
    if (dec_places)
	num_spaces -= dec_places + 1;
    thousands = fabsf(amnt / 1000.0);
    // another space for ','
    if ((int)thousands > 0)
	--num_spaces;

    printf("num_spaces: %d\n", num_spaces);
    printf("thousands: %f\n", thousands);
    // out of space, try converting thousands from comma format to K suffix
    if (num_spaces < 0) {
	// add three dig + comma but only if thousands will round up to at
	// least one, or if there are enough decimal places such that the
	// result won't be 0 when divided by a thousand
	if ((thousands > 0.50 || 
		(dec_places && dec_places < 3 &&
		 roundf(thousands*powf(10, dec_places)) > 0)) &&
		(num_spaces += 4) >= 0) {
	    printf("HERE\n");
	    suffix = 'K';
	} else {
	    *str = NULL;
	    return cur_len;
	}
    }

    // max_len + 1 for null termination
    *str = malloc((++max_len)*sizeof(char));
    if (!*str)
	return cur_len;

    if (float_eq_zero(amnt, dec_places)) {
	strncpy(*str, "--- ", 5);
	return 4;
    }

    **str = (amnt < 0.0) ? '-' : ' ';
    amnt = fabsf(amnt);
    *(*str + 1) = '$';
    cur_len = 2;

    if (suffix == 'K') {
	printf("%fl\n", thousands);
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%.*fK",
		dec_places, thousands);
	printf("%sl\n", *str);
	return cur_len;
    } 

    if ((int)thousands) {
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%d,",
		(int)thousands);
	amnt = (amnt - 1000*(int)thousands);
    }

    printf("%d\n", cur_len);
    printf("%f\n", amnt);
    if (dec_places > 0)
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%.*f ",
		dec_places, amnt);
    else
	cur_len += snprintf(*str + cur_len, max_len - cur_len, "%d ",
		(int)(roundf(amnt)));

    printf("%sl\n", *str);
    return cur_len;
}
