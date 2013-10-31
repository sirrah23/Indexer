#include "hash.h"
#define FNV_PRIME_32 16777619U
#define FNV_OFFSET_32 2166136261U

/*
 * Creates a hash table of size as indicated by the parameter, size.
 * Returns NULL if function fails.
 */
HashTablePtr makeHashTable(unsigned int size) {
    HashTablePtr table = malloc(sizeof(struct HashTable));
    
    if(table != NULL) { /*if malloc succeeds*/
        table->table_size = size;
        table->hash_table = malloc(sizeof(WordListPtr) * size); /*make space for WordList pointers, size = number of pointers*/
        if(table->hash_table != NULL) { /*if malloc succeeds*/
            int i;
            for(i = 0; i < size; i++) /*point all WordList pointers to NULL*/
                table->hash_table[i] = NULL;
        }
    }

    return table;
}

/*
 * Frees memory allocated by the hash table.
 */
void DestroyTable(HashTablePtr table) {
    /*fill in code here*/
}

/*
 * A 32-bit Fowler-Noll-Vo (FNV) hash function.
 * Returns a string's hash value.
 */
unsigned int FNV32(char *data)
{
    unsigned int hash = FNV_OFFSET_32, i;
    for(i = 0; i < strlen(data); i++)
    {
        hash = hash ^ (data[i]); // xor next byte into the bottom of the hash
        hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
    }
    return hash;
}

/*
 * Inserts a word into the hash table. Uses the Fowler-Noll-Vo (FNV) hash function.
 * Returns a pointer to a node in a linked list of words.
 */
WordListPtr TableInsert(HashTablePtr table, char *word) {
    /*fill in code here*/
}
