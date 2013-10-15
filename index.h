#include "sorted-list.h"
#include <stdlib.h>


struct Indexer{
	SortedListPtr words;
};

typedef struct Indexer* IndexerPtr;

IndexerPtr IndexerCreate(CompareFuncT cf, CompareFuncT df);

void IndexerDestroy(IndexerPtr destroyit);

int IndexerInsert(char *word, char* filename, IndexerPtr insertee);

	
