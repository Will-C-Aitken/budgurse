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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sqlite3.h>
#include <ncurses.h>

#define TIME_T_32 (sizeof(time_t) == 4)

#define MAX_CAT_BYTES 32
#define MAX_NAME_BYTES 64
#define MAX_NOTE_BYTES 64

#define MAX_AMOUNT_VAL 999999.99
#define MIN_AMOUNT_VAL -99999.99

#define KEY_ESC 27
#define KEY_ENTER 27

#define NUM_WINS 4

#define HELP_HEIGHT 12
#define HELP_WIDTH 50

#define SUMMARY_KEYS 16
#define BROWSER_KEYS 9

#define DATE_STR_LEN 10
#define NAME_STR_LEN 21
#define AMOUNT_STR_LEN 11
#define CAT_STR_LEN 18
#define SUBCAT_STR_LEN 18

typedef enum state {
    BROWSER,
    PROMPT,
    SUMMARY,
    HELP
} state_t;

typedef enum delin {
    WEEK,
    MONTH,
    YEAR,
} delin_t;

typedef enum direction {
    DOWN,
    UP,
    LEFT ,
    RIGHT
} dir_t;

#endif
