#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *invalid_chars = "\b\f\n\r\t\v\"\'\\";

/*
 * A node that holds customer information.
 */
struct Customer {
    char *name;
    int id;
    float money;
    char *address;
    char *state;
    char *zipcode;
};
typedef struct Customer* CustomerPtr;

/*
 * A database to hold all customer information.
 */
struct Database {
    CustomerPtr *table; /*A table of pointers to Customer structs*/
    int table_size;
};
typedef struct Database* DatabasePtr;

/*
 * Creates a new Customer.
 * Returns NULL if function fails.
 */
CustomerPtr makeCustomer();

/*
 * Frees any memory allocated by a Customer struct.
 */
void destroyCustomer(CustomerPtr customer);

/*
 * Creates a new database of size as indicated by the parameter.
 * Returns NULL if function fails
 */
DatabasePtr makeDatabase(int size);

/*
 * Frees any memory allocated by the database.
 */
void destroyDatabase(DatabasePtr database);

/*
 * Gets a customer's information.
 */
CustomerPtr get(DatabasePtr database, int customer_id);

/*
 * Copies contents of str2 that are not in str3 into str1.
 * Returns str1.
 */
char *copy(char *str1, char *str2, char *str3);

/*
 * Inserts a customer's information into the database.
 * Returns 1 on success and 0 on failure.
 */
int insert(DatabasePtr database, char *information);
