#include "malloc2.h"
#include <stdio.h>

int main() {
    char *test, *test1;
    test = malloc(2500);
    test1 = malloc(2536);
    free(test);
    test = malloc(2100);
    free(test1);
    free(test);
    free(test);
    free(test1);
    return 0;
}
