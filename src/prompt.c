#include "prompt.h"

void prompt_new_entry() {
    display_prompt(" Enter date (mm/dd/yyyy):");
    wrefresh(g_wins[PROMPT].win);
    // time_t date = str_to_date(get_prompt_response());
}

void display_prompt(const char* prompt_str) {
    werase(g_wins[PROMPT].win);
    wmove(g_wins[PROMPT].win, 0, 0);
    waddstr(g_wins[PROMPT].win, prompt_str);
}
