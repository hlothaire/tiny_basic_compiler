#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdbool.h>

typedef struct {
  Lexer* lexer;
  Token* currentToken;
  Token* peekToken;
  char** symbols;
  int symbolsCount;
  char** labelsDeclared;
  int labelsDeclaredCount;
  char** labelsGotoed;
  int labelsGotoedCount;
} Parser;

Parser* initParser(Lexer* lexer);
void freeParser(Parser* parser);
int checkToken(Parser* parser, TokenKind kind);
int checkPeek(Parser* parser, TokenKind kind);
void match(Parser* parser, TokenKind kind);
void nextToken(Parser* parser);
void abortParser(const char* message);
bool isComparisonOperator(Parser* parser);
void program(Parser* parser);
void statement(Parser* parser);
void comparison(Parser* parser);
void expression(Parser* parser);
void term(Parser* parser);
void unary(Parser* parser);
void primary(Parser* parser);
void nl(Parser* parser);


#endif // PARSER_H
