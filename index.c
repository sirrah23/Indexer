#include "index.h"
#include "sorted-list.h" 

IndexerPtr IndexerCreate(CompareFuncT cf, CompareFuncT df){
	IndexerPtr newindexer = malloc(sizeof(struct Indexer));
	newindexer->words = SLCreate(cf);
	newindexer->words->files = SLCreate2(df);
	return newindexer;
};

void IndexerDestroy(IndexerPtr destroyit){
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
