#include "malloc2.h"
#include <stdio.h>
#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

int main() {
    char *test;
	test = malloc(100);
    free(test);
	printf("success\n");
	test = malloc(1000);
	free(test+2);
    return 0;
}
