#include "malloc2.h"
#include <stdio.h>
#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

int main() {
    char *test, *test1;
    test = malloc(2500);
    test1 = malloc(2436);
    free(test);
    test = malloc(2100);
    free(test1);
    free(test);
    free(test);
    free(test1);
    return 0;
}
