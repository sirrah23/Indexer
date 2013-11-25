#include <stdio.h>
#include <pthread.h>
#include "database.h"
#include "queue.h"

/*Shared variables for the threads*/
DatabasePtr database;
QueuePtr sharedQ;
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
 */
void getCategories(FILE *file, char **array) {
    char buffer[300];
    int i = 0;

    while(fscanf(file, "%s", buffer) == 1) {
        /*stores contents in buffer into string*/
        char *string = malloc(sizeof(char) * (strlen(buffer)+1));
        string = strcpy(string, buffer);
        array[i] = string; /*stores string in the array*/
        i++;
    }
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

/*Builds up the initial queue */

void producerFunc(void *filename)
	pthread_mutex_lock(&mutex);	/*Lock this so queue isn't messed with during build*/
	FILE *input = (FILE) *filename;	/*Grabbing data from the orders file*/	
	char buffer[300];	/*buffer to hold each line*/
	int id, cost; /*data fields for enqueue function*/
	char *token, *title, *category;	/*data fields for enqueue + temporary token*/

	/*Grab one line at a time*/
	while(fgets(buffer, sizeof(buffer), file) != NULL){
		token = strtok(buffer, "|"); /*Temporarily store the title*/
		title = malloc(sizeof(token));	/*Malloc space for a title variable*/
		strcpy(title, token);	/*copy info from token to title*/
		cost = atof(strtok(buffer, "|");	/*Convert to float and store in cost*/
		id = atoi(strtok(buffer, "|"));		/*Conver to int and store in id*/
		token = strtok(buffer, "|"));	/*Temp store in token*/
		category = malloc(sizeof(token));	/*Malloc space for category*/
		strcpy(category, token);	/*copy info from token to category*/
		enqueue(title, cost, id, category, sharedQ);	/*Enqueue info to global queue*/
	}	
	pthread_mutex_unlock(&mutex); /*Unlock the function*/
	return NULL;
}

void consumFunc(void *categorystring){
	pthread_mutex_lock(&mutex);
	char *category = (char) *categorystring;
	float credits;
	int boolz;
	NodePtr headofqueue = peek(sharedQ);
	if(strcmp(headofqueue->list->category, category) == 0){
		headofqueue = dequeue(sharedQ);
		credits = database->table[headofqueue->id]->money;
		if(credits <= headofqueue->cost){
			database->table[headofqueue->id]->money -= credits;
			printf("Order Confirmed : %s, %f, %s %s %s %s", headofqueue->title, headofqueue->cost, database->table[headofqueue->id]->name, database->table[headofqueue->id]->address, database->table[headofqueue->id]->state, database->table[headofqueue->id]->zipcode);     
		}
		else{
			printf("Order Rejected: Customer Name : %s, Book Title:  %s, Book Cost: %f, Remaining Credits: %f", database->table[headofqueue->id]->name, headofqueue->title, headofqueue->cost, database->table[headofqueue->id]->money); 
		}
		boolz = 1;
		pthread_exit(&boolz);
	}
	boolz = 0;
	pthread_exit(&boolz);
	return;
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
	
	FILE *order_file = fopen(argv[2], "r");
	int nt, truth; /*Number thread we are currently at*/
	pthread_t producer; /*This is the thread that will produce the queue*/
	pthread_mutex_init(&mutex, NULL); /*Initialize the mutex*/
	Pthread_create(&producer, NULL, producerFunc, void *order_file); /*Run the producer thread the generate the queue*/
	while(!isEmpty(q)){	/*While the queue has stuff in it*/
		for(nt = 0; nt < num_threads; nt++){ /*Go through all the consumer threads*/
			Pthread_create(&(consumers[nt]), NULL, consumFunc, void *categories[nt]) /*Run each consumer thread*/
			pthread_join(consumers[nt], truth);
			if(truth) break;
		}
	}
	}
	pthread_mutex_destroy(&mutex); /*Destroy the mutex*/
}
