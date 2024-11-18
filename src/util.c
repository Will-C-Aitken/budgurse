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

    // trunacte string
    if (sl > max_width) {
	char trunc_str[max_width+1];
	snprintf(trunc_str, max_width+1, "%-*s", max_width, str);
	wprintw(g_wins[SUMMARY].win, "%s", trunc_str);
    } else
	wprintw(g_wins[SUMMARY].win, "%s", str);
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


