#ifndef TYPES_H 
#define TYPES_H 

#include <stdlib.h>

// lexer types ---
// morphem
typedef struct morph
{
    int id;         /*-1: empty, 0: word, 1: symbol, 2: number, 3: keyword, */
    char word[30];
    int symbol;
    int number;
    size_t lineNumber;
}MORPHEM;

// symbols enum
/* 128 = "<="
 * 129 = ">="  
 * 130 = ":="
 * 131 = "BEGIN" 
 * 132 = ""
 * 133 = "DO"
 * 134 = "END"
 * 135 = "IF"
 * 136 = "ODD"
 * 137 = "PROCEDURE"
 * 138 = "THEN"
 * 139 = "VAR"
 * 140 = "WHILE"
 * 141 = "CALL"
 * 142 = "CONST"
*/

typedef enum SYMBOLS
{
    BEGIN = 131,
    DO = 133,
    END = 134,
    IF = 135,
    ODD = 136,
    PROCEDURE = 137,
    THEN = 138,
    VAR = 139,
    WHILE = 140,
    CALL = 141,
    CONST = 142
}symbols;

enum
{
    morphemIdent = 0, 
    morphemNumeral = 2,
};

#endif