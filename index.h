#include "sorted-list.h"
#include <stdlib.h>


struct Indexer{
	SortedListPtr words;
}

typedef struct Indexer* IndexerPtr;

IndexerPtr IndexerCreate(CompareFunct cf, CompareFunct df);

void IndexerDestroy(IndexPtr destroyit);

int IndexerInsert(char *word, char* filename, IndexerPtr insertee);

	
