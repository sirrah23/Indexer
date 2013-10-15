#include "index.h"

IndexerPtr IndexerCreate(CompareFuncT cf, CompareFuncT df){
	IndexerPtr newindexer = malloc(sizeof(struct Indexer));
	newindexer->words = SLCreate(cf);
	newindexer->files = SLCreate2(df);
	return newindexer;
}

void IndexerDestroy(IndexPtr destroyit){
	SLDestroy(destroyit->words);
	free(destroyit);
}

int IndexerInsert(char* word, char* filename, IndexerPtr insertee)
{
	int x = SLInsert(insertee->words,word,filename); /*this could be wrong*/
	if(x){
		return 1;
	}
	return 0;
}
