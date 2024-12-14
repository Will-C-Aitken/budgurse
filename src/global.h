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
#include <sqlite3.h>
#include <ncurses.h>

#define TIME_T_32 (sizeof(time_t) == 4)

#define BUDGURSE_FAILURE 1
#define BUDGURSE_SUCCESS 0

#define MAX_CAT_BYTES 32
#define MAX_NAME_BYTES 64
#define MAX_NOTE_BYTES 1024

#define MAX_AMOUNT_VAL 999999.99
#define MIN_AMOUNT_VAL -99999.99

#define KEY_ESC 27

#define NUM_WINS 4

#define HELP_HEIGHT 12
#define HELP_WIDTH 50

#define SUMMARY_KEYS 16
#define BROWSER_KEYS 9

#define DATE_STR_LEN 10
#define NAME_STR_LEN 22
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
