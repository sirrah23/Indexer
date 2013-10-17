#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "tokenizer.h"
#include "index.h"

static char alphaNum[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\0";

int compareInts(int p1, int p2)
{
    return p1 - p2;
}

int compareStrings(void *p1, void *p2)
{
    char *s1 = p1;
    char *s2 = p2;

    return strcmp(s2, s1);
}

void toLowerCase(char *string) {
    int i;
    for(i = 0; string[i] != '\0'; i++)
        string[i] = tolower(string[i]);
}

/*
 * Reads a file line by line, tokenizes it, and stores the files and words into
 * a sorted list.
 */
int readFile(char *filename, IndexerPtr insertree) {
    char buffer[BUFSIZ];
    FILE *file = fopen(filename, "r");
    if(!file)
        return 0; /*failed*/

    while(fgets(buffer, sizeof(buffer), file)) { /*gets a line, puts it in buffer*/
       TokenizerT *tokenizer = TKCreate(alphaNum, buffer);
       if(tokenizer == NULL)
           return 0; /*failed*/
       char *token;
       while((token = TKGetNextToken(tokenizer)) != NULL) { /*get a token from buffer*/
            toLowerCase(token);
            if(!IndexerInsert(token, filename, insertree)) /*insert the token into the sorted list*/
                return 0; /*failed*/
       }
       TKDestroy(tokenizer);
    }
    if(fclose(file) != 0)
        return 0; /*failed*/
    return 1; /*succeded*/
}

/*
 * Reads a directory and reads all its files and its subdirectories.
 *
 * Problem: objName has 300 characters, it's doing some weird stuff when
 * written in a file.
 */
int readDirectory(char *dirName, IndexerPtr insertree/*, char *pathName*/) {
    struct dirent *dirP;
    DIR *dir;

    if((dir = opendir(dirName)) == NULL)
        return 0;

    while((dirP = readdir(dir)) != NULL) {
        if(strcmp(dirP->d_name, ".")==0 || strcmp(dirP->d_name, "..")==0)
            continue;           /*skips hidden files*/
        char objName[300];
        struct stat obj;
        sprintf(objName, "%s/%s", dirName, dirP->d_name); /*stores path name of file or another directory into objName ex: sample/test.txt or sample/directory*/
        if(stat(objName, &obj) != 0) {
            closedir(dir);
            return 0;
        }
        /*int x;*/
        if(S_ISREG(obj.st_mode)) { /*if regular file*/
            /*if(pathName == NULL) {
                x = strlen(dirP->d_name);
                char file[x] = strncpy(file, dirP->d_name, x+1);
            }*/
            if(!readFile(objName, insertree)) { /*read file*/
                closedir(dir);
                return 0;
            }
        } else if(S_ISDIR(obj.st_mode)) { /*if directory*/
            if(!readDirectory(objName, insertree)) { /*read directory*/
                closedir(dir);
                return 0; /*failed*/
            }
        }
    }
    closedir(dir);
    return 1; /*succeded*/
}

/*
 * Saves the contents of the inverted index in memory into a file.
 */
int saveFile(FILE *file, IndexerPtr index) {
    SortedListPtr temp = index->words;
    while(temp != NULL) { /*loops through all the words*/
        if(fprintf(file, "%s %s\n", "<list>", (char *)temp->data) < 0) /*write the word to the file*/
            return 0;
        SortedList2Ptr temp2 = temp->files;
        while(temp2 != NULL) { /*loops through the filenames containing the word above*/
            if(fprintf(file, "%s %d ", (char *)temp2->data, temp2->count) < 0) /*write the filename to the file*/
                return 0;
            temp2 = temp2->next;
        }
        if(fprintf(file, "\n%s\n", "</list>") < 0)
            return 0;
        temp = temp->next;
    }
    return 1;
}

/*
 * Runs the program. Tells the user what the proper input is. Opens a directory
 * or a file and writes their contents in an inverted index file.
 */
int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./index <inverted-index filename> <directory or file name>\n");
        return 1;
    }

    FILE *save = fopen(argv[1], "r");
    if(save != NULL) { /*if file already exists*/
        char input;
        while(1) {
            printf("File %s already exists, Overwrite [Y/N]? ", argv[1]);
            scanf("%c", &input);
            if(input == 'Y' || input == 'y')
                break; /*break if user wants to overwrite*/
            else if(input == 'N' || input == 'n') {
                fclose(save);
                printf("Aborting...\n"); /*abort if user doesn't want to overwrite*/
                return 1;
            }
            printf("Invalid input.\n"); /*if input is invalid*/
        }
    }

    save = fopen(argv[1], "w"); /*opens file for writing, creates a new file if file doesn't exist*/

    char *objName = argv[2];
    int successval;
    struct stat obj;
    if(stat(objName, &obj) != 0) {
        fclose(save);
        printf("Failed to read a file.\n");
        return 1;
    }

    IndexerPtr indexer = IndexerCreate(&compareStrings, &compareInts);

    if(S_ISREG(obj.st_mode)) /*if regular file*/
        successval = readFile(objName, indexer);
    else if(S_ISDIR(obj.st_mode)) /*if directory*/
        successval = readDirectory(objName, indexer);

    if(!successval) {
        IndexerDestroy(indexer);
        fclose(save);
        printf("Failed to read a file.\n");
        return 1;
    }

    if(saveFile(save, indexer)) /*saves file*/
        printf("Inverted index saved to file %s\n", argv[1]);
    else {
        IndexerDestroy(indexer);
        fclose(save);
        printf("Writing to file %s failed.\n", argv[1]);
        return 1;
    }

    IndexerDestroy(indexer);
    fclose(save);

    return 0;
}
