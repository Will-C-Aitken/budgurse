#include "test.h"

int main(int argc, char **argv) {

     #ifdef IS_UNIT_TEST
	 char *result = all_tests();

	 if (result != 0)
	     printf("%s\n", result);
	 else
	     printf("ALL UNIT TESTS PASSED\n");

	 printf("Tests run: %d\n", tests_run);
     
	 return result != 0;

    #else
	 printf("GUI test\n");
	 test_gui();
    #endif

}

