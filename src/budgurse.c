#include "budgurse.h"
#include "entries.h"

int main(int argc, char* argv[]) {
    sqlite3 *db = NULL;
    printf("return code: %d\n", open_db(db));

    entry_list_t* test_list = init_entry_list();

    entry_t* test_entry1 = init_entry("Starbucks", -12.00, "Food", 
	    "Coffee Shop", "this was fun");
    entry_node_t* test_entry_node1 = init_entry_node(test_entry1);
    append_to_tail(test_list, test_entry_node1);

    list_to_string(test_list);


    return(0);
}

int open_db(sqlite3* db) {
    int rc = sqlite3_open("budgurse.db", &db);
    
    if (rc) {
	printf("Can't open database: %s\n", sqlite3_errmsg(db));
	return(1);
    } else 
	return(0);

}
