#include "list.h"

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
WordListPtr makeWordList() {
    WordListPtr newlist = malloc(sizeof(struct WordList));  /*Mallocs space for the new list */
    if(newlist != NULL) {
        newlist->word = NULL;   /*Makes the fields of the WordListPtr to NULL*/
        newlist->next = NULL;   
        newlist->files = NULL;
    }
    return newlist;     /*Returns the list we have just created*/      
}

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
FileListPtr makeFileList() {
    FileListPtr newfilelist = malloc(sizeof(struct FileList));  /*Mallocs space for the new FileListPtr*/
    if(newfilelist != NULL) {
        newfilelist->file_name = NULL;  /*Makes the fields of the FileListPtr NULL*/
        newfilelist->next = NULL;
    }
    return newfilelist;
}

/*
 * Inserts a word into the linked list of words.
 * Returns the list.
 */
WordListPtr WLInsert(WordListPtr list, char *data) { 
    if(list->word == NULL) {     /*if the list is empty*/
        list->word = data;     /*put stuff at the head and you're done*/
        return list;
    }
    WordListPtr tobeinserted = makeWordList();     /*Otherwise make a new word list*/
    if(tobeinserted != NULL) {
        tobeinserted->word = data;    /*Place the word into the word list node*/
        tobeinserted->next = list;      /*Make the new word list node the head*/
        tobeinserted->files = NULL;
    }
    return tobeinserted;
}

/*
 * Inserts a file's name into the linked list of files.
 * Returns 1 if it succeeds, 0 if not.
 */
int FLInsert(FileListPtr list, char *data, int count) {
    if(list->file_name == NULL){     /*If the file list is empty*/
        list->file_name = data;    /*Place the data into the head and you're done*/
        list->count = count;
        return 1;
    }
    FileListPtr tobeinserted = makeFileList();  /*Else create a new file list node*/
    if(tobeinserted != NULL) {
        tobeinserted->file_name = list->file_name;           
        tobeinserted->next = list->next;                  /*Make it the head and point it to the rest of the list*/
        tobeinserted->count = list->count;
        list->file_name = data;
        list->next = tobeinserted;
        list->count = count;
        return 1;
    }
    return 0;
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
        free(p1->word);
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
        free(p1->file_name); /*Free the fields of the struct*/
        free(p1);
        p1 = p2;        /*Go to next node*/
        p2 = p1->next;
    }
    free(p1);
    return;
}
