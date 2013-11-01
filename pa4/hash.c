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
void destroyTable(HashTablePtr table) {
    int i;
    for(i = 0; i < table->table_size; i++) {
        if(table->hash_table[i] != NULL) /*frees any existing Word Lists*/
            WLDestroy(table->hash_table[i]);
    }
    
    free(table->hash_table);
    free(table);
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
 * Finds where the word is located in the table and returns the WordList pointer.
 */
 WordListPtr tableGet(HashTablePtr table, char *word) {
    unsigned int hash = FNV32(word) % table->table_size; /*calculates the hash number for the word*/
    return table->hash_table[hash]; /*returns the WordList pointer from that hash location*/
 }

/*
 * Inserts a word into the hash table. Uses the Fowler-Noll-Vo (FNV) hash function.
 * Returns a pointer to a node in a linked list of words.
 */
WordListPtr tableInsert(HashTablePtr table, char *word) {
    WordListPtr list = tableGet(table, word);
    if(list == NULL)
        list = makeWordList();
    WLInsert(list, word);
    return list;
}
