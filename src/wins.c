#include "wins.h"

win_t g_wins[NUM_WINS];

void init_wins() {
    g_wins[BROWSER] = init_browser_win();
    g_wins[PROMPT] = init_prompt_win();
    g_wins[SUMMARY] = init_summary_win();
    g_wins[HELP] = init_help_win();
}

void free_wins() {
    for (int i = 0; i < NUM_WINS; i++) 
	delwin(g_wins[i].win);
}

win_t init_browser_win() {
    win_t bw;
    bw.h = LINES - 2;
    bw.w = COLS;
    bw.y = 0;
    bw.x = 0;
    bw.win = newwin(bw.h, bw.w, bw.y, bw.x);
    keypad(bw.win, true);
    return bw;
}

win_t init_prompt_win() {
    win_t pw;
    pw.h = 2;
    pw.w = COLS;
    pw.y = LINES - pw.h;
    pw.x = 0;
    pw.win = newwin(pw.h, pw.w, pw.y, pw.x);
    keypad(pw.win, true);
    return pw;
}

win_t init_summary_win() {
    win_t sw;
    sw.h = LINES - 2;
    sw.w = COLS;
    sw.y = 0;
    sw.x = 0;
    sw.win = newwin(sw.h, sw.w, sw.y, sw.x);
    keypad(sw.win, true);
    return sw;
}

win_t init_help_win() {
    win_t hw;
    hw.h = HELP_HEIGHT;
    hw.w = HELP_WIDTH;
    hw.y = (LINES - hw.h)/2;
    hw.x = (COLS - hw.w)/2;
    hw.win = newwin(hw.h, hw.w, hw.y, hw.x);
    keypad(hw.win, true);
    return hw;
}
