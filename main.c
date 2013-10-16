#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <fts.h>
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

static int readDirectory(char * const argv[]){
	FTS *ftsp;					/*A pointer to an FTS object that holds a bunch of FTSENT*/
	FTSENT  *p, *pcheck;		/*FTSENT objects that hold all the information for directory and files*/
	int options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;	/*Options necessary for FTS to run*/

	if((ftsp = fts_open(argv, fts_options, NULL)) == NULL){		/*Takes an array of characters to and opens each directory and file in the array*/
		printf("Couldn't open this thing.");
		return -1;
	}

	pcheck = fts_children(ftsp,0);								

	if(pcheck == NULL){   /*Checks to see if there is any file to traverse*/
		return 0;
	}
	
	while((p=fts_read(ftsp)) != NULL){ /*visits each directory and file*/
		switch(p->fts_info){
			case FTS_D:		/*If directory*/
				printf("directory %s\n", p->fts_path);
				break; 
			case FTS_F:		/*If file, here is where we want to add the tokenizer and fill in insertee*/
				printf("file %s\n", p->fts_path);
				break;
			case default:
				break;


		}
	}
	fts_close(ftsp);
	return 1;


}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./index <inverted-index filename> <directory or file name>\n");
        return 1;
    }

    IndexerPtr indexer = IndexerCreate(&compareStrings, &compareInts);

    char *objName = argv[2];
	char **argfts;
	argfts[0] = *objName;
	int x = readDirectory(argfts);	
    return 0;
}
