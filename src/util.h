#ifndef UTIL_H
#define UTIL_H

#include "budgurse.h"

extern const int days_in_mnth[12]; 

// generic drawing functions
void draw_amount(WINDOW *w, float amount, int max_width, 
	const char *delim_str, int attrs);
void draw_ra_string(WINDOW *w, const char *str, int max_width,
	const char *delim_str, int attrs);
void draw_str(WINDOW *w, const char *str, int max_width, 
	const char *delim_str, int attrs);

// date time functions
int check_time_bounds(int day, int month, int year);
void clean_tm(struct tm *tm_to_clean);
int date_part_from_delin(time_t date, delin_t d);
void update_date(time_t *date, delin_t d, int amount);

// miscellaneous
int num_places_in_amount (int n);

#endif
