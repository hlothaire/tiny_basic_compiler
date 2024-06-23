#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
  Lexer *lexer;
  Token *currentToken;
  Token *peekToken;
} Parser;

Parser *initParser(Lexer *lexer);
void freeParser(Parser *parser);
int checkToken(Parser *parser, TokenKind kind);
int checkPeek(Parser *parser, TokenKind kind);
void match(Parser *parser, TokenKind kind);
void nextToken(Parser *parser);
void abortParser(Parser *parser, const char *message);

#endif // PARSER_H
