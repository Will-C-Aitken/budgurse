// from https://jera.com/techinfo/jtns/jtn002

#include <stdio.h>
#include "entries.h"
#include "backend.h"
#include "browser.h"


void test_gui();

// Unit tests

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)
extern int tests_run;

char* all_tests();

char* append_to_tail_001();
char* free_head_002();
char* free_list_003();
char* entry_to_sql_insert_004();
char* load_empty_db_005();
char* write_entry_006();
char* load_db_007();
char* where_in_list_008();

