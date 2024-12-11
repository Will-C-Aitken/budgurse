#include "util.h"

const int days_in_mnth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void draw_amount(WINDOW *w, float amount, int max_width, 
	const char *delim_str, int attrs) {
    waddstr(w, delim_str);

    // to right align
    int num_spaces, num_dig, thousands;
    num_dig = num_places_in_amount((int)amount);
    num_spaces = max_width - 5 - num_dig;
    thousands = abs((int)amount / 1000);

    // for comma
    if (thousands) 
	num_spaces--;
    while (num_spaces-- > 0) 
	waddch(w, ' ');

    // print negative (if necessary) and dollar sign 
    if (amount < 0) {
	if (attrs)
	    wattrset(g_wins[SUMMARY].win, attrs);
	waddch(w, '-');
    } else 
	waddch(w, ' ');
    if (attrs)
	wattrset(g_wins[SUMMARY].win, attrs);
    waddch(w, '$');

    amount = fabs(amount);

    // print comma in thousands
    if (thousands) 
	wprintw(w, "%d,%06.2f", thousands, 
		amount - (thousands*1000));
    else
	wprintw(w, "%0.2f", amount);

    // turn off attributes
    if (attrs)
	wattrset(g_wins[SUMMARY].win, 0);
}


void draw_ra_string(WINDOW *w, const char *str, int max_width,
	const char *delim_str, int attrs) {

    waddstr(w, delim_str);

    // to right align
    int sl = strlen(str);
    int num_spaces = (sl > max_width) ? 0 : max_width - sl;
    while (num_spaces-- > 0) 
	waddch(g_wins[SUMMARY].win, ' ');

    if (attrs)
	wattrset(g_wins[SUMMARY].win, attrs);

    // trunacte string
    if (sl > max_width) {
	char trunc_str[max_width+1];
	snprintf(trunc_str, max_width+1, "%-*s", max_width, str);
	wprintw(g_wins[SUMMARY].win, "%s", trunc_str);
    } else
	wprintw(g_wins[SUMMARY].win, "%s", str);

    // turn off attributes
    if (attrs)
	wattrset(g_wins[SUMMARY].win, 0);
}


void draw_str(WINDOW *w, const char *str, int max_width, 
	const char *delim_str, int attrs) {

    waddstr(w, delim_str);

    if (attrs)
	wattrset(g_wins[SUMMARY].win, attrs);

    // trunacte string
    char trunc_str[max_width+1];
    snprintf(trunc_str, max_width+1, "%-*s", max_width, str);
    wprintw(w, "%s", trunc_str);

    // turn off attributes
    if (attrs)
	wattrset(g_wins[SUMMARY].win, 0);
}


int num_places_in_amount(int n) {
    if (n < 0) return num_places_in_amount((n == INT_MIN) ? INT_MAX: -n);
    if (n < 10) return 1;
    return 1 + num_places_in_amount(n / 10);
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
    int date_part;
    struct tm *tm_from_date = localtime(&date);
    switch (d) {
	case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
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
	case WEEK: EXIT("Week delineation not implemented yet. Exiting\n");
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
