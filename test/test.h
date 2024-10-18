// from https://jera.com/techinfo/jtns/jtn002

#include "budgurse.h"

void test_gui();

// Unit tests
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                            if (message) return message; } while (0)
extern int tests_run;

char *all_tests();

char *append_to_tail_001();
char *free_tail_002();
char *free_entry_list_003();
char *entry_to_sql_insert_004();
char *load_empty_db_005();
char *write_entry_006();
char *load_db_007();
char *where_in_list_008();
char *browser_init_009();
char *is_end_node_010();
char *entry_node_traverse_011();
char *browser_scroll_012();
char *cat_array_013();
char *cat_id_to_names_014();
char *date_proc_015();
char *amount_proc_016();
char *cat_proc_017();
char* cat_to_sql_insert_018();
