#include <string.h>
#include <ctype.h>

/*
 * tokenizer.h
 */

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
struct TokenizerT_ {
    char* copied_string;
    char* delimiters;
    char* current_position;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * determines if a character is a member of the set of escaped characters
 */
char is_escape_character(char character);

/*
 * converts a hex digit represented as a character into its actual integer    value
 */
int char_to_hex(char character);

/*
 * converts a octal digit represented as a character into its actual integer  value
 */
int char_to_oct(char character);

/*
 * determines if a character represents a octal digit
 */
int is_oct_digit(char oct_digit);

/*
 * given an input stream converts escaped characters into their appropriate   char representation and returns it as a new string
 */
char* unescape_string(char* string);

/*
 * creates a new tokenizer struct from the token stream and delimiters
 */
TokenizerT *TKCreate(char *separators, char *ts);

/*
 * destroys tokenizer struct and deallocates all memory
 */
void TKDestroy(TokenizerT *tk);

/*
 * determines if a particular character is a member of the set of delimiters
 */
char is_delimeter(char character, char* delimeters);

/*
 * returns the next token from the token stream specified within the tokenizer
 */
char *TKGetNextToken(TokenizerT *tk);
