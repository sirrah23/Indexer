#include "list.h"

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
WordListPtr makeWordList() {
    WordListPtr newlist;
    newlist = malloc(sizeof(struct WordList));
    newlist->word = NULL;
    newlist->next = NULL;
    newlist->files = NULL;
    return newlist;
}

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
FileListPtr makeFileList() {
    FileListPtr newfilelist;
    newfilelist = malloc(sizeof(struct FileList));
    return newfilelist;
}

/*
 * Inserts a word into the linked list of words.
 * Returns 1 if it succeeds, 0 if not.
 */
int WLInsert(WordListPtr list, char *data) {
    if(list->data == NULL){
        list->word = *data;
        return 1;
    }
    WordListPtr tobeinserted = makeWorldList();
    tobeeinserted->word = *data;
    tobeinserted->next = list;
    tobeinserted->files = NULL;
    return 1;
}

/*
 * Inserts a file's name into the linked list of files.
 * Returns 1 if it succeeds, 0 if not.
 */
int FLInsert(FileListPtr list, char *data, int count) {
    if(list->data == NULL){
        list->file_name = *data;
        return 1;
    }
    FileListPtr tobeinserted = makeFileList();
    tobeeinserted->file_name = *data;
    tobeinserted->next = list;
    tobeinserted->count = count;
    return 1;
}

/*
 * Frees allocated memory for the word linked list.
 */
void WLDestroy(WordListPtr list) {
    WordListPtr p1, p2;
    p1 = list;
    p2 = p1->next;
    while(p2 != NULL){
        FLDestroy(p1->files);
        free(p1->data);
        free(p1);
        p1 = p2;
        p2 = p1->next;
    }
    free(p1);
    return;
}

/*
 * Frees allocated memory for the file linked list.
 */
void FLDestroy(FileListPtr list) {
    FileListPtr p1, p2;
    p1 = list;
    p2 = p1->next;
    while(p2 != NULL){
        free(p1->data);
        free(p1);
        p1 = p2;
        p2 = p1->next;
    }
    free(p1);
    return;
}
