#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum Tokenkind {
    EOF_TOKEN,
    NEWLINE_TOKEN,
    NUMBER_TOKEN,
    IDENT_TOKEN,
    STRING_TOKEN,
    // Keywords
    LABEL_TOKEN,
    GOTO_TOKEN,
    PRINT_TOKEN,
    INPUT_TOKEN,
    LET_TOKEN,
    IF_TOKEN,
    THEN_TOKEN,
    ENDIF_TOKEN, 
    WHILE_TOKEN,
    REPEAT_TOKEN,
    ENDWHILE_TOKEN,
    // Operators
    EQ_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    ASTERISK_TOKEN,
    SLASH_TOKEN,
    EQEQ_TOKEN,
    NOTEQ_TOKEN,
    LT_TOKEN,
    LTEQ_TOKEN,
    GT_TOKEN,
    GTEQ_TOKEN
} TokenKind;

typedef struct {
	char* text;
	TokenKind kind;
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
void printToken(Token* token);
TokenKind checkIfKeyword(const char* tokText);

#endif // !LEXER_H
