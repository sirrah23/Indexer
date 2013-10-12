#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "index.h"

int compareInts(void *p1, void *p2)
{
    int i1 = *(int*)p1;
    int i2 = *(int*)p2;

    return i1 - i2;
}

int compareStrings(void *p1, void *p2)
{
    char *s1 = p1;
    char *s2 = p2;

    return strcmp(s2, s1);
}

int readFile(char *filename, IndexerPtr insertree) {
    
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
