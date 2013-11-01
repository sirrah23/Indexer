#include "list.h"

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
WordListPtr makeWordList() {
    WordListPtr newlist;    /*Creates a new pointer*/
    newlist = malloc(sizeof(struct WordList));  /*Mallocs space for the new list */
    newlist->word = NULL;   /*Makes the fields of the WordListPtr to NULL*/
    newlist->next = NULL;   
    newlist->files = NULL;
    return newlist;     /*Returns the list we have just created*/      
}

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
FileListPtr makeFileList() {
    FileListPtr newfilelist;    /*Creates a new FileListPtr*/
    newfilelist = malloc(sizeof(struct FileList));  /*Mallocs space for the new FileListPtr*/
    newfilelist->file_name = NULL;  /*Makes the fields of the FileListPtr NULL*/
    newfilelist->next = NULL;
    newfilelist->count = 0;
    return newfilelist;
}

/*
 * Inserts a word into the linked list of words.
 * Returns 1 if it succeeds, 0 if not.
 */
int WLInsert(WordListPtr list, char *data) { 
    if(list->data == NULL){     /*if the list is empty*/
        list->word = *data;     /*put stuff at the head and you're done*/
        return 1;
    }
    WordListPtr tobeinserted = makeWorldList();     /*Otherwise make a new word list*/
    tobeeinserted->word = *data;    /*Place the word into the word list node*/
    tobeinserted->next = list;      /*Make the new word list node the head*/
    tobeinserted->files = NULL;
    return 1;
}

/*
 * Inserts a file's name into the linked list of files.
 * Returns 1 if it succeeds, 0 if not.
 */
int FLInsert(FileListPtr list, char *data, int count) {
    if(list->data == NULL){     /*If the file list is empty*/
        list->file_name = *data;    /*Place the data into the head and you're done*/
        return 1;
    }
    FileListPtr tobeinserted = makeFileList();  /*Else create a new file list node*/
    tobeeinserted->file_name = *data;           
    tobeinserted->next = list;                  /*Make it the head and point it to the rest of the list*/
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
    while(p2 != NULL){ /*Loop through the list freeing as you go*/
        FLDestroy(p1->files);   /*Free the fields of the struct*/
        free(p1->data);
        free(p1);
        p1 = p2;    /*Go to next node*/
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
    while(p2 != NULL){  /*Loop through the list freeing as you go*/
        free(p1->data); /*Free the fields of the struct*/
        free(p1);
        p1 = p2;        /*Go to next node*/
        p2 = p1->next;
    }
    free(p1);
    return;
}
