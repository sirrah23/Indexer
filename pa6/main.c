#include "malloc2.h"

int main() {
    char *test;
    test = my_malloc(1000);
    my_free(test);
    my_free(test);
    return 0;
}
