#include "sorted-list.h"

struct Indexer{
	SortedListPtr words;
};

typedef struct Indexer* IndexerPtr;

IndexerPtr IndexerCreate(CompareFuncT cf, int(*df)(int,int));

void IndexerDestroy(IndexerPtr destroyit);

int IndexerInsert(char *word, char* filename, IndexerPtr insertee);
