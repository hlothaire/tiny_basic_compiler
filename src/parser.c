#include "../includes/parser.h"


Parser* initParser(Lexer* lexer) {
  Parser* parser = (Parser*)malloc(sizeof(Parser));
  parser->lexer = lexer;
  parser->symbols = NULL;
  parser->symbolsCount = 0;
  parser->labelsDeclared = NULL;
  parser->labelsDeclaredCount = 0;
  parser->labelsGotoed = NULL;
  parser->labelsGotoedCount = 0;

  parser->currentToken = NULL;
  parser->peekToken = NULL;
  nextToken(parser);
  nextToken(parser);
  return parser;
}

int checkPeek(Parser* parser, TokenKind kind) {
  return parser->peekToken->kind == kind;
}

int checkToken(Parser* parser, TokenKind kind) {
  return parser->currentToken->kind == kind;
}

void match(Parser* parser, TokenKind kind) {
  if (!checkToken(parser,kind)) {
    char message[256];
    snprintf(message,sizeof(message), "Excepted token %d, got %d",kind,parser->currentToken->kind);
    abortParser(message);
  }
  nextToken(parser);
}

void nextToken(Parser* parser) {
  parser->currentToken = parser->peekToken;
  parser->peekToken = getToken(parser->lexer);
}

void abortParser(const char* message) {
  fprintf(stderr, "Error: %s\n",message);
  exit(EXIT_FAILURE);
}
