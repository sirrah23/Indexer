#include "index.h"

IndexerPtr IndexerCreate(CompareFuncT cf, CompareFuncT df){
	IndexerPtr newindexer = malloc(sizeof(struct Indexer));
	newindexer->words = SLCreate(cf);
	newindexer->words->files = SLCreate(df);
	return newindexer;
}

void IndexerDestroy(IndexPtr destroyit){
	SLDestroy(destroyit->words);
	free(destroyit);
}

int IndexerInsert(char* word, char* filename, IndexerPtr insertee)
{
	int x = SLInsert(insertee,word); /*this could be wrong*/
	int y = SLInsert(insertee->files,filename); /*so is this*/
	if(x && y){
		return 1;
	}
	return 0;
}
