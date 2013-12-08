struct MemEntry{
    unsigned int size;
    struct MemEntry *prev, *succ;
    unsigned char isfree;
};

void *my_malloc(unsigned int size);

void my_free(void *p);
