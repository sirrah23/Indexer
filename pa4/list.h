#include <stdlib.h>

/*
 * A node in a file linked list that holds a file's name,
 * the number of occurrences of a word in that file, and a pointer
 * to the next node.
 */
struct FileList {
    char *file_name;
    int count;
    struct FileList *next;
};
typedef struct FileList* FileListPtr;

/*
 * A node in a word linked list that holds a word, a pointer to
 * the next node, and a pointer to a linked list of files that
 * contain the word.
 */
struct WordList {
    char *word;
    struct WordList *next;
    FileListPtr files;
};
typedef struct WordList* WordListPtr;

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
WordListPtr makeWordList();

/*
 * Creates a linked list that contains a single node, which holds
 * NULL values. Returns NULL if the function fails.
 */
FileListPtr makeFileList();

/*
 * Inserts a word into the linked list of words.
 * Returns 1 if it succeeds, 0 if not.
 */
int WLInsert(WordListPtr list, char *data);

/*
 * Inserts a file's name into the linked list of files.
 * Returns 1 if it succeeds, 0 if not.
 */
int FLInsert(FileListPtr list, char *data);

/*
 * Frees allocated memory for the word linked list.
 */
void WLDestroy(WordListPtr list);

/*
 * Frees allocated memory for the file linked list.
 */
void FLDestroy(FileListPtr list);
