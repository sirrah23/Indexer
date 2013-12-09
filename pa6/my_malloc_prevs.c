#include <stdio.h>
#include "malloc2.h"
#define MEMSIZE BLOCKSIZE/sizeof(struct MemEntry)

static char big_block[BLOCKSIZE];
static char* mem_val[MEMSIZE]; //stores memory addresses of allocated memory

// return a pointer to the memory buffer requested
void* my_malloc(unsigned int size, char *file, int line)
{
	static int initialized = 0;
	static struct MemEntry *root;
	struct MemEntry *p;
	struct MemEntry *succ;
	char * result;
	int i = 0; //used for while loop to store an allocated memory address into mem_val
	
	if(!initialized) // 1st time called
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
            //store allocated memory address into mem_val
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
            //store allocated memory address into mem_val
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
	int i;
    //checks if memory address p was allocated
	for(i = 0; i < MEMSIZE; i++){
		if(mem_val[i] == (char *)p)
            break;
	}
    
    struct MemEntry *ptr;
	struct MemEntry *prev;
	struct MemEntry *succ;
	
	ptr = (struct MemEntry*)((char*)p - sizeof(struct MemEntry));
    //checks if the memory address being freed is in big_block or if it wasn't found in mem_val
    if(ptr < (struct MemEntry*)big_block || ptr > (struct MemEntry*)big_block+BLOCKSIZE || i == MEMSIZE) {
        fprintf(stderr, "Attempting to free something that wasn't allocated or was previously freed. File: %s Line: %d\n", file, line);
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

    i = 0;
    //delete the freed memory address in mem_val
	while(1){
		if(mem_val[i] == (char *)p){
			mem_val[i] = 0;
			break;
		}	
		i++;
	}
}
