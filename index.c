#include "index.h" 

IndexerPtr IndexerCreate(CompareFuncT cf, int(*df)(int,int)){
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
    void *wordPtr = word;
    void *filenamePtr = filename;
	int x = SLInsert(insertee->words,wordPtr,filenamePtr);
	if(x){
		return 1;
	}
	return 0;
}
