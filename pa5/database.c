#include "database.h"

/*
 * Creates a new successful order.
 * Returns NULL on failure.
 */
SuccOrderPtr makeSuccOrder() {
    SuccOrderPtr orders = malloc(sizeof(struct SuccOrder));
    if(orders != NULL) {
        orders->title = NULL;
        orders->price = 0;
        orders->balance = 0;
    }
    return orders;
}

/*
 * Frees any memory allocated by a SuccOrder struct.
 */
void destroySuccOrder(SuccOrderPtr orders) {
    if(orders->title != NULL)
        free(orders->title);
    free(orders);
}

/*
 * Creates a new unsuccessful order.
 * Returns NULL on failure.
 */
UnsuccOrderPtr makeUnsuccOrder() {
    UnsuccOrderPtr orders = malloc(sizeof(struct UnsuccOrder));
    if(orders != NULL) {
        orders->title = NULL;
        orders->price = 0;
    }
    return orders;
}

/*
 * Frees any memory allocated by a UnsuccOrder struct.
 */
void destroyUnsuccOrder(UnsuccOrderPtr orders) {
    if(orders->title != NULL)
        free(orders->title);
    free(orders);
}

/*
 * Creates a new Customer.
 * Returns NULL if function fails.
 */
CustomerPtr makeCustomer() {
    CustomerPtr customer = malloc(sizeof(struct Customer));
    if(customer != NULL) {
        customer->name = NULL;
        customer->id = -1;
        customer->money = 0;
        customer->address = NULL;
        customer->state = NULL;
        customer->zipcode = NULL;
        customer->succ_orders = NULL;
        customer->succ_size = 0;
        customer->unsucc_orders = NULL;
        customer->unsucc_size = 0;
    }
    return customer;
}

/*
 * Frees any memory allocated by a Customer struct.
 */
void destroyCustomer(CustomerPtr customer) {
    if(customer->name != NULL)
        free(customer->name);
    if(customer->address != NULL)
        free(customer->address);
    if(customer->state != NULL)
        free(customer->state);
    if(customer->zipcode != NULL)
        free(customer->zipcode);
    if(customer->succ_orders != NULL) {
        int i;
        for(i = 0; i < customer->succ_size; i++)
            destroySuccOrder(customer->succ_orders[i]);
    }
    if(customer->unsucc_orders != NULL) {
        int i;
        for(i = 0; i < customer->unsucc_size; i++)
            destroyUnsuccOrder(customer->unsucc_orders[i]);
    }
    free(customer);
}

/*
 * Creates a new database of size as indicated by the parameter.
 * Returns NULL if function fails
 */
DatabasePtr makeDatabase(int size) {
    DatabasePtr database = NULL;
    if(size != 0)
        database = malloc(sizeof(struct Database));
    if(database != NULL) {
        database->table = calloc(size, sizeof(CustomerPtr));
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
        return database->table[customer_id-1];
    else
        return NULL;
}

/*
 * Returns 1 if char c is included in str, 0 otherwise.
 */
int include(char c, char *str) {
    int i;
    for(i = 0; str[i] != '\0'; i++) {
        if(c == str[i])
            return 1;
    }
    return 0;
}

/*
 * Copies contents of str2 that are not in str3 into str1.
 * Returns str1.
 */
char *copy(char *str1, char *str2, char *str3) {
    int size = 0, i;

    for(i = 0; str2[i] != '\0'; i++) {
        if(!include(str2[i], str3) && str2[i]!=' ') {
            size++;
            break;
        }
    }

    str1 = malloc(sizeof(char));
    str1[size-1] = str2[i];
    i++;
    while(str2[i] != '\0') {
        if(!include(str2[i], str3)) {
            size++;
            str1 = realloc(str1, sizeof(char)*size);
            str1[size-1] = str2[i];
        } else
            break;
        i++;
    }

    str1 = realloc(str1, sizeof(char) * (size+1));
    str1[size] = '\0';
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
        return 0; /*Failed to make a customer node*/
    
    int id; float money;
    char *name = NULL, *address = NULL; 
    char *state = NULL, *zipcode = NULL;
    char *token;

    /*Stores name in the customer node*/
    if((token = strtok(information, "|")) != NULL)
        customer->name = copy(name, token, invalid_chars);
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
    if((token = strtok(NULL, "|")) != NULL)
        customer->address = copy(address, token, invalid_chars);
    /*Stores customer's state in the customer node*/
    if((token = strtok(NULL, "|")) != NULL) 
        customer->state = copy(state, token, invalid_chars);
    /*Stores customer's zipcode in the customer node*/
    if((token = strtok(NULL, "|")) != NULL)
        customer->zipcode = copy(zipcode, token, invalid_chars);
    
    /*Stores customer into the database*/
    if(customer->id != -1)
        database->table[id-1] = customer;
    else {
        destroyCustomer(customer);
        return 0;
    }
    return 1;/*Successfully stored a customer into the database*/
}
