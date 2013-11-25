#include <stdio.h>
#include <pthread.h>
#include "database.h"

/*Shared variables for the threads*/
DatabasePtr database;
pthread_mutex_t mutex;

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

/*
 * Gets the number of customers in the database.
 */
int getDatabaseSize(FILE *file) {
    char buffer[300];
    char *token;
    int id, max = 0;

    /*Checks each line in the database file*/
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        token = strtok(buffer, "|");
        while(token != NULL) {
            /*If token is an integer*/
            if(sscanf(token, "%d", &id) == 1) {
                if(id > max)
                    max = id; /*max will hold the largest customer ID*/
                break;
            }
            token = strtok(NULL, "|");
        }
    }

    rewind(file);
    return max;
}

/*
 * Reads a database file and stores it in a database in memory.
 * Returns a 1 in success, 0 otherwise.
 */
int readDatabaseFile(FILE *file, DatabasePtr database) {
  char buffer[300];

  /*Goes through each line in the file*/
  while(fgets(buffer, sizeof(buffer), file) != NULL) {
      if(!insert(database, buffer))
          return 0; /*Inserting into the database fails*/
  }

  return 1; /*On success*/
}

int main(int argc, char *argv[]) {
    if(argc != 4) { /*checks for proper user input*/
        printf("Usage: ./order <database file> <book orders file> <categories file>\nAborting\n");
        return 1;
    }

    FILE *database_file = fopen(argv[1], "r");
    if(database_file == NULL) { /*checks if file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[1]);
        return 1;
    }

    /*Initializes the database*/
    database = makeDatabase(getDatabaseSize(database_file));
    if(database == NULL) { /*Checks for initialization failure*/
        printf("Failed to make the database\nCheck file %s\nAborting\n", argv[1]);
        fclose(database_file);
        return 1;
    }

    /*Checks for failure while reading the database file*/
    if(!readDatabaseFile(database_file, database)) {
       printf("Failed to make the database\nInvalid file %s\nAborting\n", argv[1]);
       fclose(database_file);
       return 1;
    }

    fclose(database_file); /*Done with the database file*/

    FILE *categories_file = fopen(argv[3], "r");
    if(categories_file == NULL) { /*checks if file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[3]);
        return 1;
    }

    int num_threads = getSize(categories_file);
    char *categories[num_threads];
    categories = getCategories(categories_file, categories);
    fclose(categories_file);

    pthread_t consumers[num_threads]; /*holds the threads*/

    /*Do thread stuff here*/
}
