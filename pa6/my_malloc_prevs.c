#include <stdio.h>
#include "malloc2.h"


static char big_block[BLOCKSIZE];
static char* mem_val[BLOCKSIZE/sizeof(struct MemEntry)];

// return a pointer to the memory buffer requested
void* my_malloc(unsigned int size, char *file, int line)
{
	static int 		initialized = 0;
	static struct MemEntry *root;
	struct MemEntry *p;
	struct MemEntry *succ;
	char * result;
	int i = 0;
	
	if(!initialized)	// 1st time called
	{
		// create a root chunk at the beginning of the memory block
		root = (struct MemEntry*)big_block;
		root->prev = root->succ = 0;
		root->size = BLOCKSIZE - sizeof(struct MemEntry);
		root->isfree = 1;
		initialized = 1;
	}
	p = root;
	do
	{
		if(p->size < size)
		{
			// the current chunk is smaller, go to the next chunk
			p = p->succ;
		}
		else if(!p->isfree)
		{
			// this chunk was taken, go to the next
			p = p->succ;
		}
		else if(p->size < (size + sizeof(struct MemEntry)))
		{
			// this chunk is free and large enough to hold data, 
			// but there's not enough memory to hold the HEADER of the next chunk
			// don't create any more chunks after this one
			p->isfree = 0;
			result = (char*)p + sizeof(struct MemEntry); 
			while(1){
				if(mem_val[i] == 0){
					mem_val[i] = result;
					break;
				}
				i++;
			}
			return result;
		}
		else
		{
			// take the needed memory and create the header of the next chunk
			succ = (struct MemEntry*)((char*)p + sizeof(struct MemEntry) + size);
			succ->prev = p;
			succ->succ = p->succ;
			//begin add
			if(p->succ != 0)
				p->succ->prev = succ;
			p->succ = succ;
			//end add
			
			succ->size = p->size - sizeof(struct MemEntry) - size;
			p->size = size;
			p->isfree = 0;
            succ->isfree = 1;
			result = (char *)p + sizeof(struct MemEntry);
			while(1){
				if(mem_val[i] == 0){
					mem_val[i] = result;
					break;
				}
				i++;	
			}
			return result;
		}
	} while(p != 0);
	
	return 0;
}


// free a memory buffer pointed to by p
void my_free(void *p, char *file, int line)
{	
	int i = 0;
	while(1){
		if(mem_val[i] == 0 || i > (BLOCKSIZE / sizeof(struct MemEntry))){
			fprintf(stderr, "Attempting to free something that wasn't allocated. File: %s Line: %d\n", file, line);
			return;
		}
		i++;
	}

    if(p < (void*)big_block || p > (void*)big_block+BLOCKSIZE) {
        fprintf(stderr, "Freeing memory not previously allocated. File: %s Line: %d\n", file, line);
        return;
    }
    
    struct MemEntry *ptr;
	struct MemEntry *prev;
	struct MemEntry *succ;
	i = 0;
	
	ptr = (struct MemEntry*)((char*)p - sizeof(struct MemEntry));
    
    if(ptr->isfree) {
        fprintf(stderr, "Freeing memory previously freed. File: %s Line: %d\n", file, line);
        return;
    }

	if((prev = ptr->prev) != 0 && prev->isfree)
	{
		// the previous chunk is free, so
		// merge this chunk with the previous chunk
		prev->size += sizeof(struct MemEntry) + ptr->size;
		//begin add
		ptr->isfree=1;
		prev->succ = ptr->succ;
		if(ptr->succ != 0)
			ptr->succ->prev = prev;
		//end add
	}
	else
	{
		ptr->isfree = 1;
		prev = ptr;	// used for the step below
	}
	
	if((succ = ptr->succ) != 0 && succ->isfree)
	{
		// the next chunk is free, merge with it
		prev->size += sizeof(struct MemEntry) + succ->size;
		prev->isfree = 1;
		//begin add
		prev->succ = succ->succ;
		if(succ->succ != 0)
			succ->succ->prev=prev;
		//end add
	}
	while(1){
		if(mem_val[i] == (char *)p){
			mem_val[i] = 0;
			break;
		}	
		i++;
	}
}
