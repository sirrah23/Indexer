#include "malloc2.h"

int main() {
    //test 1
    char *test, *test1;
    printf("----- START TEST1 -----\n");
    test = malloc(300);
    test1 = malloc(4600);
    free(test);
    free(test1);
    printf("------ END TEST1 ------\n");

    //test2
    char *test2, *test3;
    printf("----- START TEST2 -----\n");
    free(test2);
    test3 = malloc(5005);
    free(test3);
    printf("------ END TEST2 ------\n");

    //test3
    char *test4, *test5, *test6;
    printf("----- START TEST3 -----\n");
    test4 = malloc(300);
    test5 = malloc(1000);
    test6 = malloc(1000);
    free(test5 + 5);
    free(test4);
    free(test4);
    free(test5);
    free(test6);
    printf("------ END TEST3 ------\n");

    //test4
    char *test7, *test8, *test9, *test10;
    printf("----- START TEST4 -----\n");
    test7 = malloc(200);
    test8 = malloc(400);
    test9 = malloc(600);
    test10 = malloc(800);
    free(test10);
    free(test9);
    free(test8);
    free(test7);
    test7 = malloc(200);
    test8 = malloc(400);
    test9 = malloc(600);
    test10 = malloc(800);
    free(test10);
    free(test9);
    free(test8);
    free(test7);
    test7 = malloc(200);
    test8 = malloc(400);
    test9 = malloc(600);
    test10 = malloc(800);
    free(test10);
    free(test9);
    free(test8);
    free(test7);
    printf("------ END TEST4 ------\n");

    //test5
    char *test11;
    printf("----- START TEST5 -----\n");
    test11 = malloc(100);
    free(test11);
    test11 = malloc(100);
    free(test11);
    printf("------ END TEST5 ------\n");

    return 0;
}
