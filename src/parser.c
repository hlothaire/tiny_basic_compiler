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

bool isComparisonOperator(Parser* parser) {
  return checkToken(parser, GT_TOKEN) || checkToken(parser, GTEQ_TOKEN) || checkToken(parser, LT_TOKEN) || 
           checkToken(parser, LTEQ_TOKEN) || checkToken(parser, EQEQ_TOKEN) || checkToken(parser, NOTEQ_TOKEN);
}

void program(Parser* parser) {
  printf("PROGRAM\n");

  while (checkToken(parser, NEWLINE_TOKEN)) {
    nextToken(parser);
  }

  while (!checkToken(parser, EOF_TOKEN)) {
    statement(parser);
  }

  for (int i = 0; i < parser->labelsGotoedCount; i++) {
    bool found = false;
    for (int j = 0; j < parser->labelsDeclaredCount; j++) {
      if (strcmp(parser->labelsGotoed[i], parser->labelsDeclared[j]) == 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      char error[100];
      sprintf(error,"Attempting to GOTO undeclared label: %s", parser->labelsGotoed[i]);
      abortParser(error);
    }
  }
}
