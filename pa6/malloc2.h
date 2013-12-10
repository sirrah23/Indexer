#include <stdio.h>
#define BLOCKSIZE 5000
#define MEMSIZE BLOCKSIZE/sizeof(struct MemEntry)
#define malloc(x) my_malloc(x, __FILE__, __LINE__)
#define free(x) my_free(x, __FILE__, __LINE__)

struct MemEntry{
    unsigned int size;
    struct MemEntry *prev, *succ;
    unsigned char isfree;
};

void *my_malloc(unsigned int size, char *file, int line);

void my_free(void *p, char *file, int line);
