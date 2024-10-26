#ifndef BUDGURSE_H
#define BUDGURSE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sqlite3.h>
#include <ncurses.h>

#define TIME_T_32 (sizeof(time_t) == 4)

#define BUDGURSE_FAILURE 1
#define BUDGURSE_SUCCESS 0

#define MAX_NAME_BYTES 64
#define MAX_CAT_BYTES 32
#define MAX_NOTE_BYTES 1024

#define MAX_AMOUNT_VAL 999999.99
#define MIN_AMOUNT_VAL -99999.99

#define KEY_ESC 27

typedef enum state {
    BROWSER,
    PROMPT,
    SUMMARY,
} state_t;

extern state_t state;
extern int curses_mode;

// the following three macros are adapted from calcurse/src/calcurse.h
#define ERROR_MSG(...) do {                                                  \
    char msg[BUFSIZ];                                                        \
    int len;                                                                 \
    len = snprintf(msg, BUFSIZ, "%s: %d: ", __FILE__, __LINE__);             \
    snprintf(msg + len, BUFSIZ - len, __VA_ARGS__);                          \
    fprintf(stderr, "%s\n", msg);                                            \
} while (0)

#define EXIT(...) do {							     \
    ERROR_MSG(__VA_ARGS__);                                                  \
    if (curses_mode)							     \
	end_budgurse(EXIT_FAILURE);                                          \
    else								     \
	exit(EXIT_FAILURE);						     \
} while (0)

#define EXIT_IF(cond, ...) do {                                              \
    if ((cond))                                                              \
	EXIT(__VA_ARGS__);                                                   \
} while (0)

void init_budgurse();
int handle_input();
void end_budgurse();

#include "entries.h"
#include "backend.h"
#include "browser.h"
#include "categories.h"
#include "wins.h"
#include "prompt.h"
#include "summary.h"

#endif
