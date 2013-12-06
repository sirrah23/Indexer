#include <stdio.h>
#define blocksize 5000

char big_block[blocksize];

void minimalloc(unsigned int size){
    static int initialized = 0;
    static struct mementry *root, *p, *next;

    if(!initialized){
        root = big_block;
        root->size = block_sizeof(struct mementry);
        root->prev = root->succ = 0;
        root->isfree = 1;
        initialized = 1;
    }

    p = root;
    
    do{
        if(p->size < size)
            p = p->succ;
        else if(p->isfree == 0)
            p = p->succ;
        else if(p->size < (size + sizeof(struct mementry))){
            p->isfree = 0;
            return (char *) p + sizeof(struct mementry);
        }
        else{
            next = (struct mementry*)((char*)p*sizeof(struct mementry) + size); 
            p->succ->prev = next;
            next->prev = p;
            next->succ = p->succ;
            next->size = p->size - sizeof(struct mementry) - size;
            next->isfree = 1;
            p->size = size;
            p->isfree = 0;
            return (char*)p + sizeof(struct mementry);
        }
    }while(p != 0);
    return 0;
}
