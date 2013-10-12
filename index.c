#include "index.h"

IndexerPtr IndexerCreate(CompareFuncT cf, CompareFuncT df){
	IndexerPtr newindexer = malloc(sizeof(struct Indexer));
	newindexer->words = malloc(sizeof(struct SortedList));
	newindexer->words = SLCreate(cf);
	newindexer->words->files = malloc(sizeof(struct SortedList));
	new->index->words->files = SLCreate(df);
	return newindexer;
}

void IndexerDestroy(IndexPtr destroyit){
	SLDestroy(destroyit->words);
	free(destroyit);
}

int IndexerInsert(char* word, char* filename, IndexerPtr insertee)
{
	int x = SLInsert(insertee,word);
	int y = SLInsert(insertee->files,filename);
	if(x && y){
		return 1;
	}
	return 0;
}

