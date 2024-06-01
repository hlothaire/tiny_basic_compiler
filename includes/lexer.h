#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenType {
    EOF_TOKEN = -1,
    NEWLINE_TOKEN = 0,
    NUMBER_TOKEN = 1,
    IDENT_TOKEN = 2,
    STRING_TOKEN = 3,
    // Keywords
    LABEL_TOKEN = 101,
    GOTO_TOKEN = 102,
    PRINT_TOKEN = 103,
    INPUT_TOKEN = 104,
    LET_TOKEN = 105,
    IF_TOKEN = 106,
    THEN_TOKEN = 107,
    ENDIF_TOKEN = 108,
    WHILE_TOKEN = 109,
    REPEAT_TOKEN = 110,
    ENDWHILE_TOKEN = 111,
    // Operators
    EQ_TOKEN = 201,
    PLUS_TOKEN = 202,
    MINUS_TOKEN = 203,
    ASTERISK_TOKEN = 204,
    SLASH_TOKEN = 205,
    EQEQ_TOKEN = 206,
    NOTEQ_TOKEN = 207,
    LT_TOKEN = 208,
    LTEQ_TOKEN = 209,
    GT_TOKEN = 210,
    GTEQ_TOKEN = 211
} TokenType;

typedef struct {
	char* text;
	TokenType type;
} Token;

typedef struct {
	char* source;
	char curChar;
	int curPos;
} Lexer;


Lexer *initLexer(const char* input);
void freeLexer(Lexer* lexer);
void nextChar(Lexer* lexer);
char peek(Lexer* lexer);
void skipWhitespace(Lexer* lexer);
void skipComment(Lexer* lexer);
void abortLexing(Lexer* lexer, const char* message);
Token* getToken(Lexer* lexer);
int isKeyword(char *text);
void printToken(Token* token);

#endif // !LEXER_H
