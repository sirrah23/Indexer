#include "list.h"

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
WordListPtr makeWordList() {
    /*fill in code here*/
}

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
FileListPtr makeFileList() {
    /*fill in code here*/
}

/*
 * Inserts a word into the linked list of words.
 * Returns 1 if it succeeds, 0 if not.
 */
int WLInsert(WordListPtr list, char *data) {
    /*fill in code here*/
}

/*
 * Inserts a file's name into the linked list of files.
 * Returns 1 if it succeeds, 0 if not.
 */
int FLInsert(FileListPtr list, char *data) {
    /*fill in code here*/
}

/*
 * Frees allocated memory for the word linked list.
 */
void WLDestroy(WordListPtr list) {
 /*fill in code here*/
}

/*
 * Frees allocated memory for the file linked list.
 */
void FLDestroy(FileListPtr list) {
    /*fill in code here*/
}
