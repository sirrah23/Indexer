#include "malloc2.h"
#include <stdio.h>

int main() {
    char *test;
    test = malloc(100);
    free(test);
    test = malloc(100);
    free(test);



    return 0;
}
