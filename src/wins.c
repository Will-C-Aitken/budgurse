#include "wins.h"

win_t g_wins[NUM_WINS];

void init_wins() {
    g_wins[BROWSER] = init_browser_win();
    g_wins[PROMPT] = init_prompt_win();
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
    return bw;
}

win_t init_prompt_win() {
    win_t pw;
    pw.h = 2;
    pw.w= COLS;
    pw.y= LINES - pw.h;
    pw.x= 0;
    pw.win = newwin(pw.h, pw.w, pw.y, pw.x);
    return pw;
}
