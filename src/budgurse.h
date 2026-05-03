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

#ifndef BUDGURSE_H
#define BUDGURSE_H

#include "global.h"
#include "entries.h"
#include "categories.h"
#include "summary.h"
#include "wins.h"
#include "browser.h"
#include "help.h"
#include "date.h"
#include "backend.h"

extern int curses_mode;
extern state_t state;

typedef budgurse {
//     browser_t *browser;
//     summary_t *summary;
//     win_t wins[NUM_WINS];
//     llist_t *categories;
//     entry_list_t *entry_list;
    sqlite3 *db;
//     help_t *help;
// 
//     int curses_mode;
//     state_t state;
} budgurse_t;

void init_budgurse(budgurse_t **b);
int handle_input(budgurse_t *b);
void draw();
int resize();
void end_budgurse(budgurse_t *b);

#endif
