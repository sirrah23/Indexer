#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
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
int readFile(char *pathname, IndexerPtr insertree, char *filename) {
    char buffer[BUFSIZ];
    FILE *file = fopen(pathname, "r");
    if(!file)
        return 0; /*failed*/

    while(fgets(buffer, sizeof(buffer), file)) { /*gets a line, puts it in buffer*/
       TokenizerT *tokenizer = TKCreate(alphaNum, buffer);
       if(tokenizer == NULL) {
           fclose(file);
           return 0; /*failed*/
       }
       char *token;
       while((token = TKGetNextToken(tokenizer)) != NULL) { /*get a token from buffer*/
            toLowerCase(token);
            if(!IndexerInsert(token, filename, insertree)) { /*insert the token into the sorted list*/
                fclose(file);
                return 0; /*failed*/
            }
       }
       TKDestroy(tokenizer);
    }
    fclose(file);
    return 1; /*succeded*/
}

/*
 * Reads a directory and reads all its files and its subdirectories.
 *
 * Problem: objName has 300 characters, it's doing some weird stuff when
 * written in a file.
 */
int readDirectory(char *dirName, IndexerPtr insertree, char *pathName) {
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
        if(S_ISREG(obj.st_mode)) { /*if regular file*/
            char file[300];
            if(pathName == NULL)
                sprintf(file, "%s", dirP->d_name);
            else
                sprintf(file, "%s%s", pathName, dirP->d_name);
            if(!readFile(objName, insertree, file)) { /*read file*/
                closedir(dir);
                return 0;
            }
        } else if(S_ISDIR(obj.st_mode)) { /*if directory*/
            int x = strlen(dirP->d_name);
            if(pathName == NULL) {
                char path[x+2];
                sprintf(path, "%s/", dirP->d_name);
                if(!readDirectory(objName, insertree, path)) { /*read directory*/
                    closedir(dir);
                    return 0; /*failed*/
                }
            } else {
                int y = strlen(pathName);
                char path[y+x+2];
                sprintf(path, "%s%s/", pathName, dirP->d_name);
                if(!readDirectory(objName, insertree, path)) { /*read directory*/
                    closedir(dir);
                    return 0; /*failed*/
                }
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
        printf("Usage: ./index <inverted-index filename> <directory or file name>\nAborting\n");
        return 1;
    }

    FILE *save = fopen(argv[1], "r");
    if(save != NULL) { /*if file already exists*/
        char input[10];
        while(1) {
            printf("File %s already exists, Overwrite [Y/N]? ", argv[1]);
            scanf("%s", input);
            toLowerCase(input);
            if(strcmp(input, "yes") == 0 || strcmp(input, "y") == 0)
                break; /*break if user wants to overwrite*/
            else if(strcmp(input, "no") == 0 || strcmp(input, "n") == 0) {
                fclose(save);
                printf("Aborting\n"); /*abort if user doesn't want to overwrite*/
                return 1;
            }
            printf("Invalid input.\n"); /*if input is invalid*/
        }
        fclose(save);
    }

    save = fopen(argv[1], "w"); /*opens file for writing, creates a new file if file doesn't exist*/

    char *objName = argv[2];
    int successval;
    struct stat obj;
    if(stat(objName, &obj) != 0) {
        fclose(save);
        printf("Failed to read a file.\nAborting\n");
        return 1;
    }

    IndexerPtr indexer = IndexerCreate(&compareStrings, &compareInts);

    if(S_ISREG(obj.st_mode)) { /*if regular file*/
        char *obj = malloc(sizeof(char)*(strlen(objName)+1));
        obj = strcpy(obj, objName);
        successval = readFile(objName, indexer, obj);
    } else if(S_ISDIR(obj.st_mode)) /*if directory*/
        successval = readDirectory(objName, indexer, NULL);

    if(!successval) {
        IndexerDestroy(indexer);
        fclose(save);
        printf("Failed to read a file.\nAborting\n");
        return 1;
    }

    if(saveFile(save, indexer)) /*saves file*/
        printf("Inverted index saved to file %s\n", argv[1]);
    else {
        IndexerDestroy(indexer);
        fclose(save);
        printf("Writing to file %s failed.\nAborting\n", argv[1]);
        return 1;
    }

    IndexerDestroy(indexer);
    fclose(save);

    return 0;
}
