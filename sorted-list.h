#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */

#include <stdlib.h>

struct SortedList2
{   
    void *data;
    struct SortedList2* next;
    int (*comparator)(int, int);
    int count;
};

typedef struct SortedList2* SortedList2Ptr;

/*
 * Sorted list type.  You need to fill in the type as part of your implementation.
 */
struct SortedList
{
	void *data;
	struct SortedList2* files;
	struct SortedList* next;
	int (*comparator)(void *, void *);
};

typedef struct SortedList* SortedListPtr;


/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 *
 * You can expect a comparator function to return -1 if the 1st object is
 * smaller, 0 if the two objects are equal, and 1 if the 2nd object is
 * smaller.
 *
 * Note that you are not expected to implement any comparator functions.
 * You will be given a comparator function when a new sorted list is
 * created.
 */

typedef int (*CompareFuncT)(void *, void *);

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf);

SortedList2Ptr SLCreate2(int(*cf)(int,int));

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list);

void SLDestroy2(SortedList2Ptr list);

int SLInsert2(SortedList2Ptr list, void *newObj, CompareFuncT comp);

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj, void *newObj2);

#endif
