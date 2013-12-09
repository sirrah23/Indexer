#define BLOCKSIZE 5000

struct MemEntry{
    unsigned int size;
    struct MemEntry *prev, *succ;
    unsigned char isfree;
};

void *my_malloc(unsigned int size, char *file, int line);

void my_free(void *p, char *file, int line);
