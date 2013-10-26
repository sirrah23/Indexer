#include <stdio.h>
#include "hash.h"

/*
 * Gets the number of words that are in the inverted-index.
 */
unsigned int getIndexSize(FILE *file) {
    /*fill in code here*/
}

int main(int argc, char *argv[]) {
    if(argc != 2) { /*Only 2 arguments are allowed*/
        printf("Usage: ./search <inverted-index file name>\nAborting\n");
        return 1;
    }

    FILE *index_file = fopen(argv[1], "r");
    if(index_file == NULL) { /*checks if the inverted-index file exists*/
        printf("Failed to read file %s or file %s does not exist\nAborting\n", argv[1], argv[1]);
        return 1;
    }
    
    HashTablePtr hash_table = makeHashTable(getIndexSize(index_file));
    if(hash_table == NULL) { /*if it fails to malloc memory for the hash table*/
        printf("Failed to read the inverted-index into memory\nAborting\n");
        return 1;
    }
}
