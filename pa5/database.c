#include "database.h"

/*
 * Creates a new Customer.
 * Returns NULL if function fails.
 */
CustomerPtr makeCustomer() {
    CustomerPtr customer = malloc(sizeof(struct Customer));
    return customer;
}

/*
 * Frees any memory allocated by a Customer struct.
 */
void destroyCustomer(CustomerPtr customer) {
    free(customer->name); free(customer->address);
    free(customer->state); free(customer->zipcode);
    free(customer);
}

/*
 * Creates a new database of size as indicated by the parameter.
 * Returns NULL if function fails
 */
DatabasePtr makeDatabase(int size) {
    DatabasePtr database = malloc(sizeof(struct Database));
    if(database != NULL) {
        database->table = calloc(sizeof(CustomerPtr) * size);
        database->table_size = size;
    }
    return database;
}

/*
 * Frees any memory allocated by the database.
 */
void destroyDatabase(DatabasePtr database) {
    int i;
    for(i = 0; i < database->table_size; i++) {
        if(database->table[i] != NULL)
            destroyCustomer(database->table[i]);
    }
    free(database->table);
    free(database);
}

/*
 * Gets a customer's information.
 */
CustomerPtr get(DatabasePtr database, int customer_id) {
    if(customer_id <= database->table_size)
        return database->table[customer_id];
    else
        return NULL;
}

/*
 * Copies n characters from str2 to str1 starting at position
 * offset in str2. Returns str1.
 */
char *copy(char *str1, char *str2, int offset, int n) {
    int i;
    for(i = 0; i < n; i++)
        str1[i] = str2[i+offset];
    str1[i] = '\0';
    return str1;
}

/*
 * Inserts a customer's information into the database.
 * Returns 1 on success and 0 on failure.
 */
int insert(DatabasePtr database, char *information) {
    /*Make a customer node*/
    CustomerPtr customer;
    if((customer = makeCustomer()) == NULL)
        return 0;
    
    int id; float money;
    char *name, *address, *state, *zipcode;
    char *token;

    /*Stores name in the customer node*/
    if((token = strtok(information, "|")) != NULL) {
        name = malloc(sizeof(char) * (strlen(token)-1));
        customer->name = copy(name, token, 1, strlen(token)-2);
    }
    /*Stores customer ID in the customer node*/
    if((token = strtok(NULL, "|")) != NULL) {
        if(sscanf(token, "%d", &id) == 1)
            customer->id = id;
    }
    /*Stores money amount in the customer node*/
    if((token = strtok(NULL, "|")) != NULL) {
        if(sscanf(token, "%f", &money) == 1)
            customer->money = money;
    }
    /*Stores address in the customer node*/
    if((token = strtok(NULL, "|")) != NULL) {
        address = malloc(sizeof(char) * (strlen(token)-1));
        customer->address = copy(address, token, 1, strlen(token)-2);
    }
    /*Stores customer's state in the customer node*/
    if((token = strtok(NULL, "|")) != NULL) {
        state = malloc(sizeof(char) * (strlen(token)-1));
        customer->state = copy(state, token, 1, strlen(token)-2);
    }
    /*Stores customer's zipcode in the customer node*/
    if((token = strtok(NULL, "|")) != NULL) {
        zipcode = malloc(sizeof(char) * (strlen(token)-1));
        customer->zipcode = copy(zipcode, token, 1, strlen(token)-2);
    }
    
    /*Stores customer into the database*/
    database->table[id] = customer;
    return 1;
}
