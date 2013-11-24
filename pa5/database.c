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
