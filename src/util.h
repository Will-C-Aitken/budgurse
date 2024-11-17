#ifndef UTIL_H
#define UTIL_H

#include "budgurse.h"

// generic drawing functions
void draw_amount(WINDOW *w, float amount, int max_width, 
	const char *delim_str, int attrs);
void draw_ra_string(WINDOW *w, const char *str, int max_width,
	const char *delim_str, int attrs);
void draw_str(WINDOW *w, const char *str, int max_width, 
	const char *delim_str, int attrs);

// miscellaneous
int num_places_in_amount (int n);
int check_time_bounds(int day, int month, int year);

#endif
