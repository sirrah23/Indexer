#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "database.h"
#include "queue.h"

/*Shared variables for the threads*/
DatabasePtr database;
QueuePtr sharedQ;
pthread_mutex_t mutex;
float totalrev = 0;
int procorders = 0;

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
 * Frees memory allocated by the Categories array.
 */
void destroyCategories(char **array, int size) {
    int i;
    for(i = 0; i < size; i++)
        free(array[i]);
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

void printFinal() {
    FILE *save = fopen("finalreport.txt", "w");
    int i;
    for(i = 0; i < database->table_size; i++) {
        CustomerPtr customer = database->table[i];
        printf("=== BEGIN CUSTOMER INFO ===\n");
        fprintf(save, "%s\n", "=== BEGIN CUSTOMER INFO ===");
        printf("### BALANCE ###\n");
        fprintf(save, "%s\n", "### BALANCE ###");
        printf("Customer name: %s\n", customer->name);
        fprintf(save, "%s %s\n", "Customer name:", customer->name);
        printf("Customer ID number: %d\n", customer->id);
        fprintf(save, "%s %d\n", "Customer ID number:", customer->id);
        printf("Remaining balance after purchases: %.2f\n", customer->money);
        fprintf(save, "%s %.2f\n", "Remaining balance after purchases:", customer->money);
        printf("### SUCCESSFUL ORDERS ###\n");
        fprintf(save, "%s\n", "### SUCCESSFUL ORDERS ###");
        int y;
        for(y = 0; y < customer->succ_size; y++) {
            SuccOrderPtr order = customer->succ_orders[y];
            printf("\"%s\"| %.2f| %.2f\n", order->title, order->price, order->balance);
            fprintf(save, "\"%s\"| %.2f| %.2f\n", order->title, order->price, order->balance);
        }
        printf("### REJECTED ORDERS ###\n");
        fprintf(save, "%s\n", "### REJECTED ORDERS ###");
        for(y = 0; y < customer->unsucc_size; y++) {
            UnsuccOrderPtr order = customer->unsucc_orders[y];
            printf("\"%s\"| %.2f\n", order->title, order->price);
            fprintf(save, "\"%s\"| %.2f\n", order->title, order->price);
        }
        printf("=== END CUSTOMER INFO ===\n");
        fprintf(save, "%s\n", "=== END CUSTOMER INFO ===");
        if(i < database->table_size-1) {
            printf("\n");
            fprintf(save, "\n");
        }
    }
    fclose(save);
}

/*Builds up the initial queue */

void *producerFunc(void *filename){
	FILE *input = (FILE *) filename ;	/*Grabbing data from the orders file*/	
	char buffer[1000];	/*buffer to hold each line*/
	int id;/*data fields for enqueue function*/
	float cost;
	char *token, *title = NULL, *category = NULL;	/*data fields for enqueue + temporary token*/

	/*Grab one line at a time*/
	while(fgets(buffer, sizeof(buffer), input) != NULL){
		token = strtok(buffer, "|"); /*Temporarily store the title*/
		title = copy(title, token, invalid_chars);
		token = strtok(NULL, "|");
		cost = atof(token);	/*Convert to float and store in cost*/
		token = strtok(NULL, "|");
		id = atoi(token);		/*Conver to int and store in id*/
		token = strtok(NULL, " |");	/*Temp store in token*/
		category = copy(category, token, invalid_chars);
		pthread_mutex_lock(&mutex);	/*Lock this so queue isn't messed with during build*/
		enqueue(title, cost, id, category, sharedQ);	/*Enqueue info to global queue*/
		pthread_mutex_unlock(&mutex); /*Unlock the function*/
	}
		fclose(input);
		procorders = 1;
		pthread_exit(NULL);
}

void *consumFunc(void *categorystring){
	NodePtr temp;
	while(1){
		pthread_mutex_lock(&mutex);
		if(isEmpty(sharedQ) && procorders == 1){
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
		else if(isEmpty(sharedQ)){
			pthread_mutex_unlock(&mutex);
			/*sleep(1);*/
		}
		else{
			temp = peek(sharedQ);
			if(strcmp(temp->category, categorystring) != 0){
				pthread_mutex_unlock(&mutex);
				/*sleep(1);*/
			}
			else{
				temp = dequeue(sharedQ);
                CustomerPtr customer = get(database, temp->id);
				if(customer->money >= temp->cost){
					customer->money -= temp->cost;
					printf("Order Confirmed! Book Title: %s, Price: %.2f, Name: %s, Address: %s %s %s\n", temp->title, temp->cost, customer->name, customer->address, customer->state, customer->zipcode);
					totalrev += temp->cost;
                    if(customer->succ_size == 0)
                        customer->succ_orders = malloc(sizeof(SuccOrderPtr));
                    else
                        customer->succ_orders = realloc(customer->succ_orders, sizeof(SuccOrderPtr) * (customer->succ_size+1));
                    SuccOrderPtr orders = makeSuccOrder();
                    if(orders != NULL) {
                        orders->title = temp->title;
                        orders->price = temp->cost;
                        orders->balance = customer->money;
                    }
                    customer->succ_orders[customer->succ_size] = orders;
                    customer->succ_size++;
					pthread_mutex_unlock(&mutex);
				}
				else{
					printf("Order Rejected! Customer: %s, Book Title: %s, Price: %.2f, Remaining Balance: %.2f\n", customer->name, temp->title, temp->cost, customer->money);
                    if(customer->unsucc_size == 0)
                        customer->unsucc_orders = malloc(sizeof(UnsuccOrderPtr));
                    else
                        customer->unsucc_orders = realloc(customer->unsucc_orders, sizeof(UnsuccOrderPtr) * (customer->unsucc_size+1));
                    UnsuccOrderPtr orders = makeUnsuccOrder();
                    if(orders != NULL) {
                        orders->title = temp->title;
                        orders->price = temp->cost;
                    }
                    customer->unsucc_orders[customer->unsucc_size] = orders;
                    customer->unsucc_size++;
					pthread_mutex_unlock(&mutex);
				}
			}
		}
	}
    pthread_exit(NULL);
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
    getCategories(categories_file, categories);
    fclose(categories_file);

    pthread_t consumers[num_threads]; /*holds the threads*/
	int nt;/*Number thread we are currently at*/
	FILE *order_file = fopen(argv[2], "r");
    if(order_file == NULL) { /*checks if file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[2]);
        return 1;
    }
	sharedQ = createQueue();
    pthread_attr_t attr;
	pthread_t producer; /*This is the thread that will produce the queue*/
	pthread_mutex_init(&mutex, NULL); /*Initialize the mutex*/
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    void *status;
	pthread_create(&producer, &attr, producerFunc, (void *)order_file); /*Run the producer thread the generate the queue*/
	for(nt = 0; nt < num_threads; nt++){ /*Go through all the consumer threads*/
		pthread_create(&(consumers[nt]), &attr, consumFunc,(void *) categories[nt]); /*Run each consumer thread*/
	}
    pthread_attr_destroy(&attr);

    pthread_join(producer, &status);
    for(nt = 0; nt < num_threads; nt++){
        pthread_join(consumers[nt], &status);
    }
	pthread_mutex_destroy(&mutex); /*Destroy the mutex*/
    
    printf("\n****** Total Revenue is $%.2f ******\n\n", totalrev);
    printFinal();

    destroyDatabase(database);
    destroyCategories(categories, num_threads);
    clearQueue(sharedQ); 

	pthread_exit(NULL);	
}
