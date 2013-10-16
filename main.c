#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
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

int readFile(char *filename, IndexerPtr insertree) {
    char buffer[BUFSIZ];
    FILE *file = fopen(filename, "r");
    if(!file)
        return 0;

    while(fgets(buffer, sizeof(buffer), file)) {
       TokenizerT *tokenizer = TKCreate(alphaNum, buffer);
       if(tokenizer == NULL)
           return 0;
       char *token;
       while((token = TKGetNextToken(tokenizer)) != NULL) {
            toLowerCase(token);
            if(!IndexerInsert(token, filename, insertree)
                return 0;
       }
       TKDestroy(tokenizer);
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./index <inverted-index filename> <directory or file name>\n");
        return 1;
    }

    IndexerPtr indexer = IndexerCreate(&compareStrings, &compareInts);

    char *objName = argv[2];
    struct stat obj;
    stat(objName, &obj);
    if(S_ISREG(obj.st_mode))
        printf("File is regular.\n");
    else if (S_ISDIR(obj.st_mode))
        printf("File is a directory.\n");

    return 0;
}
