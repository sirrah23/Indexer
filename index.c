#include <string.h>
#include "index.h" 

/*Creates an IndexerStruct by mallocing space for the 
 * SortedList that is inside of it 
*/

IndexerPtr IndexerCreate(CompareFuncT cf, int(*df)(int,int)){
	IndexerPtr newindexer = malloc(sizeof(struct Indexer));
	newindexer->words = SLCreate(cf);
	newindexer->words->files = SLCreate2(df);
	return newindexer;
};

/*Calls the SLDestroy function for the SortedList to free malloced space*/

void IndexerDestroy(IndexerPtr destroyit){
	SLDestroy(destroyit->words);
	free(destroyit);
}

/*Inserts into the Indexer struct by inserting into the SortedList using SLInsert*/


int IndexerInsert(char* word, char* filename, IndexerPtr insertee)
{
    void *wordPtr = word;
    char *file = malloc(sizeof(char)*(strlen(filename)+1));
    file = strcpy(file, filename);
    void *filenamePtr = file;
	int x = SLInsert(insertee->words,wordPtr,filenamePtr);
	if(x){
		return 1;
	}
	return 0;
}
