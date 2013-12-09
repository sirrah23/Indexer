#include "malloc2.h"
#include <stdio.h>
#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

int main() {
    char *test;
	test = malloc(10000);
    return 0;
}
