#include "prompt.h"

int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

const char *date_prompt = "Enter date "
    "(mm/dd/yyyy or mm/dd for current year):";
const char *name_prompt = "Enter descriptor (cannot be empty):";
const char *amnt_prompt = "Enter amount:";
const char *m_cat_prompt = "Enter category:";
const char *s_cat_prompt = "Enter subcategory (leave empty for none):";
const char *note_prompt = "Enter note (leave empty for none):";


int prompt_new_entry() {

    time_t date;
    char name[MAX_NAME_BYTES];
    float amnt; 
    int cat_id;
    char note[MAX_NOTE_BYTES];

    if ((get_entry_input(date_prompt, &date, (input_proc_fn_t)date_proc) || 
         get_entry_input(name_prompt, name, (input_proc_fn_t)name_proc) ||
         get_entry_input(amnt_prompt, &amnt, (input_proc_fn_t)amount_proc) ||
         get_entry_input(m_cat_prompt, &cat_id, (input_proc_fn_t)m_cat_proc) ||
	 get_entry_input(s_cat_prompt, &cat_id, (input_proc_fn_t)s_cat_proc) ||
         get_entry_input(note_prompt, note, (input_proc_fn_t)note_proc))
	== BUDGURSE_FAILURE) {

	werase(g_wins[PROMPT].win);
	wrefresh(g_wins[PROMPT].win);
	return 1; 
    }

    entry_t *e = init_entry(g_entries->next_free_id, name, date, amnt, cat_id, 
	    (strlen(note) == 0) ? NULL : note);
    db_exec(e, (gen_sql_fn_t)entry_to_sql_insert);
    entry_node_t* en = init_entry_node(e);
    append_to_tail(g_entries, en);

    werase(g_wins[PROMPT].win);
    wrefresh(g_wins[PROMPT].win);
    return 0;
}


int prompt_add_category(const char *cat_name, int parent_id) {
    int ch;
    category_t *new_cat = NULL;
    const char *add_cat_str = "The entered category does not exist. Would "
	"you like to add it? (y/n)";

    while (1) {
	display_prompt(add_cat_str, 0, true);
	ch = wgetch(g_wins[PROMPT].win);
	if (ch == 'y') {
	    int new_cat_id = get_next_id(g_categories);
	    new_cat = init_category(new_cat_id, parent_id, cat_name);
	    append_to_cat_array(g_categories, new_cat);
	    db_exec(new_cat, (gen_sql_fn_t)cat_to_sql_insert);
	    return new_cat_id;
	} 
	if (ch == 'n')
	    return 0;
    }
}

void prompt_edit_entry(entry_node_t *cur) {
    int ch, rc = 1;
    const char *edit_prompt = "Edit: (1) Date, (2) Name, (3) Amount, "
	"(4) Categories, (5) Note";

    display_prompt(edit_prompt, 0, true);

    while (rc == BUDGURSE_FAILURE) {
	ch = wgetch(g_wins[PROMPT].win);
	switch (ch) {
	    case '1': {
		time_t new_date;
		if ((rc = get_entry_input(date_prompt, &new_date, 
			(input_proc_fn_t)date_proc)) == BUDGURSE_SUCCESS)
		    entry_set_date(cur->data, new_date);
		break;
	    } 
	    case '2': {
		char new_name[MAX_NAME_BYTES];
		if ((rc = get_entry_input(name_prompt, new_name, 
			(input_proc_fn_t)name_proc)) == BUDGURSE_SUCCESS)
		    entry_set_name(cur->data, new_name);
		break;
	    } 
	    case '3': {
		float new_amount;
		if ((rc = get_entry_input(amnt_prompt, &new_amount, 
			(input_proc_fn_t)amount_proc)) == BUDGURSE_SUCCESS)
		    entry_set_amount(cur->data, new_amount);
		break;
	    } 
	    case '4': {
		int new_id;
		if ((rc = get_entry_input(m_cat_prompt, &new_id, 
			(input_proc_fn_t)m_cat_proc)) == BUDGURSE_FAILURE)
		    break;
		if ((rc = get_entry_input(s_cat_prompt, &new_id, 
			(input_proc_fn_t)s_cat_proc)) == BUDGURSE_SUCCESS)
		    entry_set_cat_id(cur->data, new_id);
		break;
	    } 
	    case '5': {
		char new_note[MAX_NOTE_BYTES];
		if ((rc = get_entry_input(note_prompt, new_note, 
			(input_proc_fn_t)note_proc)) == BUDGURSE_SUCCESS)
		    entry_set_note(cur->data, new_note);
		break;
	    } 
	    case 'q':
	    case KEY_ESC:
		werase(g_wins[PROMPT].win);
		wrefresh(g_wins[PROMPT].win);
		return;
	}
    }

    db_exec(cur->data, (gen_sql_fn_t)edit_entry_to_sql_update);
    werase(g_wins[PROMPT].win);
    wrefresh(g_wins[PROMPT].win);
}


int get_entry_input(const char *prompt_str, void *output, 
	input_proc_fn_t p_fn) {

    const char *err_str = "Invalid format. Press 'q' to cancel entry or any "
	"other key to try again";
    char response[BUFSIZ];
    int poll = 1;

    while (poll) {
	response[0] = '\0';
	display_prompt(prompt_str, 0, true);

	if (!get_prompt_response(response))
	    poll = p_fn(response, output);

	if (poll) {
	    display_prompt(err_str, 0, false);
	    int ch = wgetch(g_wins[PROMPT].win);
	    if (ch == 'q')
		return 1;
	}
    }

    return 0;
}


void display_prompt(const char *prompt_str, int line, bool refresh) {
    if (refresh)
	werase(g_wins[PROMPT].win);
    wmove(g_wins[PROMPT].win, line, 1);
    waddstr(g_wins[PROMPT].win, prompt_str);
    wrefresh(g_wins[PROMPT].win);
}


int get_prompt_response(char* pr) {
    int rc;
    wmove(g_wins[PROMPT].win, 1, 1);
    echo();
    rc = wgetnstr(g_wins[PROMPT].win, pr, BUFSIZ);
    noecho();
    return rc;
}

int date_proc(char *buf, time_t *date) {

    //month
    int data[3] = {0, 0, 0};
    // initialize m, d, y to invalid date 0
    int lens[3] = {2, 2, 4}; 
    char *ch = buf, *end_ptr;
    char temp[5];

    // loop through the three date attributes
    for (int i = 0; i < 3; i++) {

	// loop through the correct number of characters for each
	for (int j = 0; j < lens[i]; j++) {
	    if ((*ch < '0') || (*ch > '9'))
		return 1;
	    temp[j] = *ch;
	    ch++;
	}
	
	// null terminate for conversion
	temp[lens[i]] = '\0';
	data[i] = strtol(temp, &end_ptr, 10);
	if (end_ptr == temp)
	    return 1;

	// The only characters allowed between digits are '/' and terminator
	if (*ch == '/') 
	    ch++;
	else {
	    if (*ch == '\0') {
		// no year is okay
		if (i == 1) {
		    time_t current_time = time(NULL);
		    struct tm current_tm = *localtime(&current_time);
		    data[2] = current_tm.tm_year + 1900;
		    break;
		}
		if (i != 2)
		    return 1;
	    }
	    else
		return 1;
	}

    }
    
    if (check_time_bounds(data[1], data[0], data[2]))
	return 1;

    struct tm result_tm = {.tm_mday=data[1],
			   .tm_mon=--data[0],
			   .tm_year=data[2] - 1900};

    *date = mktime(&result_tm);
    return 0;
}

int name_proc(char *buf, char *name) {
    if (strlen(buf) == 0)
	return 1;
    snprintf(name, MAX_NAME_BYTES, "%s", buf);
    return 0;
}


int amount_proc(char *buf, float *amount) {
    if (strlen(buf) == 0)
	return 1;

    char *end_ptr = buf;
    *amount = strtof(buf, &end_ptr);

    return (*end_ptr != '\0' || *amount == 0.0 || 
	    *amount > MAX_AMOUNT_VAL || *amount < MIN_AMOUNT_VAL);
}

int m_cat_proc(char *buf, int *id) {
    if (cat_proc(buf, id, true))
	return 1;
    if (!is_sub_cat(g_categories, *id, 0))
	return 1;
    return 0;
}


int s_cat_proc(char *buf, int *id) {
    int m_id = *id;
    // empty string is legal, just return the id (it's already set to parent
    // cat)
    if (strlen(buf) == 0)
	return 0;
    if (cat_proc(buf, id, false))
	return 1;
    if (!is_sub_cat(g_categories, *id, m_id)) {
	return 1;
    }
    return 0;
}


int cat_proc(char *buf, int *id, bool is_main_cat) {
    int p_id = (is_main_cat) ? 0 : *id;
    int min_len = (is_main_cat) ? 1 : 0;
    char cat_name[MAX_CAT_BYTES];

    if (snprintf(cat_name, MAX_CAT_BYTES, "%s", buf) < min_len)
	return 1;

    *id = cat_name_to_id(g_categories, cat_name, is_main_cat);
    
    // Could not find cat_id
    if (*id == 0) {
	if (curses_mode) {
	    *id = prompt_add_category(cat_name, p_id);
	    return (*id == 0);
	} else
	    return 1;
    }
    return 0;
}


int note_proc(char *buf, char *note) {
    if (strlen(buf) == 0)
	return 0;
    snprintf(note, MAX_NOTE_BYTES, "%s", buf);
    return 0;
}


int check_time_bounds(int day, int month, int year) {
    // If system has 32 bit time_t, year must be less than 2038
    if (TIME_T_32) 
	if (year < 1900 || year > 2037) return 1;
    if (month < 1 || month > 12) 
	return 1;
    if (day < 0 || day > days[month-1])
	return 1;
    return 0;
}


// int scan_cat(const char *buf, char* cat_name, char* subcat_name) {
//     // first check if slash for subcat
//     if 
//     // (max int is 10 chars + %)*2 + /*2 + ^ + [ + ] + s \0 = 29
//     // round up to 32
//     char format[32]; 
//     snprintf(format, (MAX_CAT_BYTES-1)*2, "%%%d[^/]/%%%ds", MAX_CAT_BYTES-1,
// 	    MAX_CAT_BYTES-1);
//     // returns number of matched fields which as to be 2
//     return (sscanf(buf, format, cat_name, subcat_name) != 2);
// }
