struct mementry{
    unsigned int size;
    struct mementry *prev, *succ;
    unsigned char isfree;
}

void *minimalloc(unsigned int size);

void *minifree(void *);


