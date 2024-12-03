#ifndef WINS_H
#define WINS_H

#include "budgurse.h"

#define NUM_WINS 3

typedef struct win {
    WINDOW *win;
    int h;
    int w;
    int x;
    int y;
} win_t;

extern win_t g_wins[NUM_WINS];

void init_wins();
void free_wins();
win_t init_browser_win();
win_t init_prompt_win();
win_t init_summary_win();

#endif
