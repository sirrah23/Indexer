#include <stdio.h>
#include <ctype.h>
#include "hash.h"
#define ERROR_MESSAGE "Failed to read the inverted-index into memory\nAborting\n"

/*
 * Gets the number of words in the inverted-index file.
 */
unsigned int getIndexSize(FILE *file) {
    char buffer[300];
    unsigned int size = 0;
    
    while(fscanf(file, "%s", buffer) == 1) { /*goes through each string in the file*/
        if(strcmp(buffer, "<list>") == 0) /*counts the number of distinct words*/
            size++;
    }
     
    rewind(file); /*puts pointer in the file to point to the very beginning*/
    return size*2; /*size is twice the number of words to reduce hash collisions*/
}

/*
 * Converts all the characters in the string into lowercase
 * characters.
 */
void toLowerCase(char *string) {
    int i;
    for(i = 0; string[i] != '\0'; i++)
        string[i] = tolower(string[i]);
}

/*
 * Reads the entries in the inverted-index file and stores
 * them into memory in the form of a hash table.
 * Returns 1 if the fuction succeeds, 0 otherwise.
 */
int readFile(FILE *file, HashTablePtr table) {
    WordListPtr temp;
    char buffer[300];
    char *word; int count;
    while(fscanf(file,"%s",buffer)){
       if(strcmp(buffer,"<list>") == 0){
            if(fscanf(file,"%s",buffer)){
                word = malloc(sizeof(char)*strlen(buffer));
                word = strcpy(word,buffer);
                temp = tableInsert(table,word);
                while(fscanf(file,"%s",buffer)){
                    if(strcmp(buffer,"</list>") == 0){
                        break;
                    }
                    free(word)
                    word = malloc(sizeof(char)*strlen(buffer));
                    word = strcpy(word,buffer);
                    FLInsert(temp->files,word);
                    free(word);
                    fscanf(file,"%s",buffer);
                    word = malloc(sizeof(char)*strlen(buffer));
                    temp->files->count = atoi(word);
                }
            }
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if(argc != 2) { /*Only 2 arguments are allowed*/
        printf("Usage: ./search <inverted-index file name>\nAborting\n");
        return 1;
    }

    FILE *index_file = fopen(argv[1], "r");
    if(index_file == NULL) { /*checks if the inverted-index file exists*/
        printf("Failed to read file %s or this file does not exist\nAborting\n", argv[1]);
        return 1;
    }
    
    HashTablePtr hash_table = makeHashTable(getIndexSize(index_file));
    if(hash_table == NULL) { /*if it fails to malloc memory for the hash table*/
        printf(ERROR_MESSAGE);
        return 1;
    }
    
    if(!readFile(index_file, hash_table)) { /*tries to read the file*/
        printf(ERROR_MESSAGE); /*if it fails, print the error message and destroy the hash table*/
        DestroyTable(hash_table);
        return 1;
    }

    while(1) { /*keeps looping until user quits*/
        printf("Enter sa <term/s>... to get files that contain all the term/s\n");
        printf("Enter so <term/s>... to get files that contain 1 or more of the term/s\n");
        printf("Enter q to quit\n> ");
        
        char buffer[300]; /*holds user input*/
        fgets(buffer, sizeof(buffer), stdin); /*puts user input into buffer*/
        buffer[strlen(buffer)-1] = '\0'; /*removes new line character (\n)*/
        toLowerCase(buffer);
        
        if(strcmp(buffer,"quit") == 0 || strcmp(buffer,"q") == 0) {
            DestroyTable(hash_table); /*if user quits, free allocated memory*/
            printf("Program exited\n");
            return 0; /*breaks out of while loop and returns successful exit to main*/
        }

        FileListPtr *file_list = NULL;
        int size = 0;
        char *token;
        token = strtok(buffer, " ");
        if(strcmp(token, "sa") == 0) {
            while((token = strtok(NULL, " ")) != NULL) {
                WordListPtr temp = tableGet(hash_table, token);
                if(temp == NULL) {
                    if(file_list != NULL) {
                        free(file_list);
                        file_list = NULL;
                    }
                    break;
                }
                if(size == 0)
                    file_list = malloc(sizeof(FileListPtr));
                else
                    file_list = realloc(file_list, sizeof(FileListPtr)*(size+1));
                file_list[size] = temp->files;
                size++;
            }
        }
    }
}
