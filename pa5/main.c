#include <stdio.h>
#include <pthread.h>

/*
 * Returns the number of categories.
 */
int getSize(FILE *file) {
    /*still working on this*/
}

int main(int argc, char *argv[]) {
    if(argc != 4) { /*checks for proper user input*/
        printf("Usage: ./order <database file> <book orders file> <categories file>\nAborting\n");
        return 1;
    }

    FILE *categories = fopen(argv[3], "r");
    if(categories == NULL) { /*checks if file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[3]);
        return 1;
    }

    int num_threads = getSize(categories);
}
