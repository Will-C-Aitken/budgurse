#ifndef BUDGURSE_H
#define BUDGURSE_H

#include "llist.h"
#include "util.h"
#include "backend.h"
#include "browser.h"
#include "categories.h"
#include "wins.h"
#include "summary.h"
#include "help.h"
#include "global.h"

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

extern int curses_mode;
extern state_t state;

void init_budgurse();
int handle_input();
void draw(state_t s);
void end_budgurse();

#endif
