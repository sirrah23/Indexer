#include <stdio.h>
#include <string.h>
#include <pthread.h>

/*
 * Returns the number of categories.
 */
int getSize(FILE *file) {
    char buffer[300];
    int size = 0;
    
    while(fscanf(file, "%s", buffer) == 1)
        size++;

    rewind(file);
    return size;
}

/*
 * Stores the categories in the array.
 * Returns the array.
 */
char **getCategories(FILE *file, char **array) {
    char buffer[300];
    int i = 0;

    while(fscanf(file, "%s", buffer) == 1) {
        /*stores contents in buffer into string*/
        char *string = malloc(sizeof(char) * (strlen(buffer)+1));
        string = strcpy(string, buffer);
        array[i] = string; /*stores string in the array*/
        i++;
    }

    return array;
}

int main(int argc, char *argv[]) {
    if(argc != 4) { /*checks for proper user input*/
        printf("Usage: ./order <database file> <book orders file> <categories file>\nAborting\n");
        return 1;
    }

    FILE *categories_file = fopen(argv[3], "r");
    if(categories_file == NULL) { /*checks if file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[3]);
        return 1;
    }

    int num_threads = getSize(categories_file);
    char *categories[num_threads];
    categories = getCategories(categories_file, categories);
    fclose(categories_file);

    /*make the database*/

    pthread_t consumers[num_threads]; /*holds the threads*/

    /*Do thread stuff here*/
}
