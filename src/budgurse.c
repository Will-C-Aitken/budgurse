#include "budgurse.h"

int main(int argc, char* argv[]) {
    sqlite3 *db;
    printf("return code: %d\n", open_db(db));

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
