#include "malloc2.h"
#include <stdio.h>

int main() {
    char *test, *test1, *test2, *test3, *test4;
    /*test1*/
    test = malloc(300);
    test1 = malloc(4600);
    free(test);
    free(test1);

    /*test2*/
    free(test);
    test1 = malloc(5005);
    free(test1);

    /*test3*/
    test = malloc(300);
    test2 = malloc(1000);
    test3 = malloc(1000);
    free(test2 + 5);
    free(test);
    free(test);
    free(test2);
    free(test3);

    /*test4*/
    test = malloc(200);
    test2 = malloc(400);
    test3 = malloc(600);
    test4 = malloc(800);
    free(test4);
    free(test3);
    free(test2);
    free(test);
    test = malloc(200);
    test2 = malloc(400);
    test3 = malloc(600);
    test4 = malloc(800);
    free(test4);
    free(test3);
    free(test2);
    free(test);
    test = malloc(200);
    test2 = malloc(400);
    test3 = malloc(600);
    test4 = malloc(800);
    free(test4);
    free(test3);
    free(test2);
    free(test);

    /*test5*/
    test = malloc(100);
    free(test);
    test = malloc(100);
    free(test);



    return 0;
}
