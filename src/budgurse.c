#include "budgurse.h"

int main(int argc, char* argv[]) {
    sqlite3 *db = NULL;
    int result = init_db(&db);
    printf("Result: %d\n", result);

    return 0;
}

