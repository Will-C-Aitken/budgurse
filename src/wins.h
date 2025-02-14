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

#ifndef WINS_H
#define WINS_H

#include "global.h"

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
int wins_resize();
win_t init_browser_win();
win_t init_prompt_win();
win_t init_summary_win();
win_t init_help_win();

// popup windows are not saved in global array of windows
win_t *init_popup_win();
void free_popup_win(win_t *puw);

#endif
