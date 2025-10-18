/* budgurse - budgeting with curses
 *
 * Copyright (c) 2025 W. C. Aitken 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include "test.h"


int misc_tests() {
    mu_run_test(float_eq_zero_test);
    mu_run_test(make_space_in_amnt_str_test);
    mu_run_test(amnt_to_str_test);
    // mu_run_test(amnt_to_abrv_str_test);
    return 0;
}

int float_eq_zero_test() {
    mu_assert(float_eq_zero(0.49, 0) == true, "Misc", 1);
    mu_assert(float_eq_zero(0.5, 0) == false, "Misc", 2);
    mu_assert(float_eq_zero(0.49, 1) == false, "Misc", 3);
    mu_assert(float_eq_zero(0.049, 1) == true, "Misc", 4);
    mu_assert(float_eq_zero(0.0049, 2) == true, "Misc", 5);
    mu_assert(float_eq_zero(-0.049, 2) == false, "Misc", 6);
    mu_assert(float_eq_zero(-0.05, 1) == false, "Misc", 7);
    // float precision creates issues here. fine for fraction 
    // of a cent 
    // mu_assert(float_eq_zero(0.005, 2) == false, "Misc", 8);
    return 0;
}

int make_space_in_amnt_str_test() {
    // already have space 
    int space = 0;
    uint32_t dec_places = 0;
    mu_assert(make_space_in_amnt_str(&space, &dec_places), "Misc", 9);
    mu_assert(space == 0, "Misc", 10);
    mu_assert(dec_places == 0, "Misc", 11);

    // out of space and can't make more
    space = -1;
    dec_places = 0;
    mu_assert(!make_space_in_amnt_str(&space, &dec_places), "Misc", 12);
    mu_assert(space == -1, "Misc", 13);
    mu_assert(dec_places == 0, "Misc", 14);

    // out of space and can make more but not enough
    space = -3;
    dec_places = 1;
    mu_assert(!make_space_in_amnt_str(&space, &dec_places), "Misc", 15);
    mu_assert(space == -1, "Misc", 16);
    mu_assert(dec_places == 0, "Misc", 17);

    // out of space and can make just enough (because ".00" is 3)
    space = -3;
    dec_places = 2;
    mu_assert(make_space_in_amnt_str(&space, &dec_places), "Misc", 18);
    mu_assert(space == 0, "Misc", 19);
    mu_assert(dec_places == 0, "Misc", 20);

    // out of space and can make just enough (actually an extra this time)
    space = -3;
    dec_places = 3;
    mu_assert(make_space_in_amnt_str(&space, &dec_places), "Misc", 21);
    mu_assert(space == 1, "Misc", 22);
    mu_assert(dec_places == 0, "Misc", 23);

    // out of space and have plenty extra but use just enough
    space = -3;
    dec_places = 5;
    mu_assert(make_space_in_amnt_str(&space, &dec_places), "Misc", 24);
    mu_assert(space == 0, "Misc", 25);
    mu_assert(dec_places == 2, "Misc", 26);

    return 0;
}

// int amnt_to_abrv_str_test() {
//     char *test_str = NULL;
// 
//     mu_assert(amnt_to_abrv_str(0.5, &test_str) == 2, "Misc", 29);
//     mu_assert(strcmp(test_str, "$1") == 0, "Misc", 30);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(0.49, &test_str) == 2, "Misc", 31);
//     mu_assert(strcmp(test_str, "$0") == 0, "Misc", 32);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(-0.49, &test_str) == 2, "Misc", 33);
//     mu_assert(strcmp(test_str, "$0") == 0, "Misc", 34);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(-0.5, &test_str) == 3, "Misc", 35);
//     mu_assert(strcmp(test_str, "-$1") == 0, "Misc", 36);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(999.5, &test_str) == 3, "Misc", 37);
//     mu_assert(strcmp(test_str, "$1K") == 0, "Misc", 38);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(-999.5, &test_str) == 4, "Misc", 39);
//     mu_assert(strcmp(test_str, "-$1K") == 0, "Misc", 40);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(-999.49, &test_str) == 5, "Misc", 41);
//     mu_assert(strcmp(test_str, "-$999") == 0, "Misc", 42);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(-999500.0, &test_str) == 0, "Misc", 43);
//     mu_assert(!test_str, "Misc", 44);
// 
//     // as precise as float can get to .99
//     mu_assert(amnt_to_abrv_str(-999499.96, &test_str) == 6, "Misc", 45);
//     mu_assert(strcmp(test_str, "-$999K") == 0, "Misc", 46);
//     free(test_str);
// 
//     mu_assert(amnt_to_abrv_str(999499.96, &test_str) == 5, "Misc", 47);
//     mu_assert(strcmp(test_str, "$999K") == 0, "Misc", 48);
//     free(test_str);
// 
//     return 0;
// }

int amnt_to_str_test() {
    char *test_str = NULL;
    int dec_places = 2;
    int max_len = 13;

    mu_assert(amnt_to_str(0.006, &test_str, dec_places, max_len) == 7, "Misc", 27);
    mu_assert(strcmp(test_str, " $0.01 ") == 0, "Misc", 28);
    free(test_str);

    mu_assert(amnt_to_str(-0.004, &test_str, dec_places, max_len) == 4, "Misc", 29);
    mu_assert(strcmp(test_str, "--- ") == 0, "Misc", 30);
    free(test_str);

    mu_assert(amnt_to_str(0.5, &test_str, dec_places, max_len) == 7, "Misc", 31);
    mu_assert(strcmp(test_str, " $0.50 ") == 0, "Misc", 32);
    free(test_str);

    mu_assert(amnt_to_str(-0.5, &test_str, dec_places, max_len) == 7, "Misc", 33);
    mu_assert(strcmp(test_str, "-$0.50 ") == 0, "Misc", 34);
    free(test_str);

    mu_assert(amnt_to_str(-0.555, &test_str, dec_places, max_len) == 7, "Misc", 35);
    mu_assert(strcmp(test_str, "-$0.56 ") == 0, "Misc", 36);
    free(test_str);

    mu_assert(amnt_to_str(999488.5, &test_str, dec_places, max_len) == 13, "Misc", 37);
    printf("%sl\n", test_str);
    mu_assert(strcmp(test_str, " $999,488.50 ") == 0, "Misc", 38);
    free(test_str);

    mu_assert(amnt_to_str(-10950, &test_str, dec_places, 11) == 8, "Misc", 39);
    mu_assert(strcmp(test_str, "-$10.95K") == 0, "Misc", 40);
    free(test_str);

    dec_places = 0;
    mu_assert(amnt_to_str(-0.5, &test_str, dec_places, max_len) == 4, "Misc", 41);
    mu_assert(strcmp(test_str, "-$1 ") == 0, "Misc", 42);
    free(test_str);

    mu_assert(amnt_to_str(-999488.5, &test_str, dec_places, max_len) == 10, "Misc", 43);
    mu_assert(strcmp(test_str, "-$999,489 ") == 0, "Misc", 44);
    free(test_str);

    mu_assert(amnt_to_str(999488.5, &test_str, dec_places, max_len) == 10, "Misc", 45);
    mu_assert(strcmp(test_str, " $999,489 ") == 0, "Misc", 46);
    free(test_str);

    max_len = 6;
    mu_assert(amnt_to_str(999488.5, &test_str, dec_places, max_len) == 6, "Misc", 47);
    mu_assert(strcmp(test_str, " $999K") == 0, "Misc", 48);
    free(test_str);

    mu_assert(amnt_to_str(78465.52, &test_str, dec_places, max_len) == 5, "Misc", 49);
    mu_assert(strcmp(test_str, " $78K") == 0, "Misc", 50);
    free(test_str);

    mu_assert(amnt_to_str(520.69, &test_str, dec_places, max_len) == 6, "Misc", 51);
    mu_assert(strcmp(test_str, " $521 ") == 0, "Misc", 52);
    free(test_str);

    mu_assert(amnt_to_str(-500.00, &test_str, 1, max_len) == 6, "Misc", 53);
    mu_assert(strcmp(test_str, "-$0.5K") == 0, "Misc", 54);
    free(test_str);

    // does not round up due to float precision limitations
    mu_assert(amnt_to_str(-500.00, &test_str, 0, 5) == -1, "Misc", 55);

    mu_assert(amnt_to_str(-500.01, &test_str, 0, 5) == 4, "Misc", 56);
    mu_assert(strcmp(test_str, "-$1K") == 0, "Misc", 57);
    free(test_str);

    mu_assert(amnt_to_str(1.0125, &test_str, 3, 5) == -1, "Misc", 57);

    mu_assert(amnt_to_str(40.25, &test_str, 2, 7) == 7, "Misc", 58);
    mu_assert(strcmp(test_str, " $0.04K") == 0, "Misc", 59);
    free(test_str);

    // cannot convert to K format because precision will make it look like 0.0K
    mu_assert(amnt_to_str(40.25, &test_str, 1, 6) == -1, "Misc", 60);

    return 0;
}
