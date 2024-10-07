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

typedef enum state {
    BROWSER,
    PROMPT,
} state_t;

extern state_t state;

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
    end_budgurse(EXIT_FAILURE);                                              \
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

#endif
