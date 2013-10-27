#include <string.h>
#include "list.h"

/*
 * A struct that holds the hash table and table size.
 */
struct HashTable {
    WordListPtr *hash_table; /*A table of pointers to WordList structs*/
    unsigned int table_size; /*Will be used as the hash size for the hashing function*/
};
typedef struct HashTable* HashTablePtr;

/*
 * Creates a hash table of size as indicated by the parameter, size.
 * Returns NULL if function fails.
 */
HashTablePtr makeHashTable(unsigned int size);

/*
 * Frees memory allocated by the hash table.
 */
void DestroyTable(HashTablePtr table);

/*
 * Inserts a word into the hash table. Uses the Fowler-Noll-Vo (FNV) hash function.
 * Returns a pointer to a node in a linked list of words.
 */
WordListPtr TableInsert(HashTablePtr table, char *word);
