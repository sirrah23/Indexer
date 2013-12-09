#include "malloc2.h"
#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

int main() {
    char *test;
    char *test1;
    test = malloc(1000);
    free(test);
    free(test);
    free(test1);
    return 0;
}
