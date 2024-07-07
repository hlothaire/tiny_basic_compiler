#include "../includes/parser.h"

Parser *initParser(Lexer *lexer) {
  Parser *parser = (Parser *)malloc(sizeof(Parser));
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

int checkPeek(Parser *parser, TokenKind kind) {
  return parser->peekToken->kind == kind;
}

int checkToken(Parser *parser, TokenKind kind) {
  return parser->currentToken->kind == kind;
}

void match(Parser *parser, TokenKind kind) {
  if (!checkToken(parser, kind)) {
    char message[256];
    snprintf(message, sizeof(message), "Excepted token %d, got %d", kind,
             parser->currentToken->kind);
    abortParser(message);
  }
  nextToken(parser);
}

void nextToken(Parser *parser) {
  parser->currentToken = parser->peekToken;
  parser->peekToken = getToken(parser->lexer);
}

void abortParser(const char *message) {
  fprintf(stderr, "Error: %s\n", message);
  exit(EXIT_FAILURE);
}

bool isComparisonOperator(Parser *parser) {
  return checkToken(parser, GT_TOKEN) || checkToken(parser, GTEQ_TOKEN) ||
         checkToken(parser, LT_TOKEN) || checkToken(parser, LTEQ_TOKEN) ||
         checkToken(parser, EQEQ_TOKEN) || checkToken(parser, NOTEQ_TOKEN);
}

void program(Parser *parser) {
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
      sprintf(error, "Attempting to GOTO undeclared label: %s",
              parser->labelsGotoed[i]);
      abortParser(error);
    }
  }
}

void statement(Parser *parser) {
  if (checkToken(parser, PRINT_TOKEN)) {
    printf("STATEMENT-PRINT\n");
    nextToken(parser);

    if (checkToken(parser, STRING_TOKEN)) {
      nextToken(parser);
    } else {
      expression(parser);
    }
  } else if (checkToken(parser, IF_TOKEN)) {
    printf("STATEMENT-IF\n");
    nextToken(parser);
    comparison(parser);
    match(parser, THEN_TOKEN);
    nl(parser);

    while (!checkToken(parser, ENDIF_TOKEN)) {
      statement(parser);
    }
    match(parser, ENDIF_TOKEN);
  } else if (checkToken(parser, WHILE_TOKEN)) {
    printf("STATEMENT-WHILE\n");
    nextToken(parser);
    comparison(parser);
    match(parser, REPEAT_TOKEN);
    nl(parser);

    while (!checkToken(parser, ENDWHILE_TOKEN)) {
      statement(parser);
    }
    match(parser, ENDWHILE_TOKEN);
  } else if (checkToken(parser, LABEL_TOKEN)) {
    printf("STATEMENT-LABEL\n");
    nextToken(parser);

    for (int i = 0; i < parser->labelsDeclaredCount; i++) {
      if (strcmp(parser->currentToken->text, parser->labelsDeclared[i]) == 0) {
        abortParser("Label already exists");
      }
    }
    parser->labelsDeclared =
        realloc(parser->labelsDeclared,
                sizeof(char *) * (parser->labelsDeclaredCount + 1));
    parser->labelsDeclared[parser->labelsDeclaredCount] =
        strdup(parser->currentToken->text);
    parser->labelsDeclaredCount++;
    match(parser, IDENT_TOKEN);
  } else if (checkToken(parser, GOTO_TOKEN)) {
    printf("STATEMENT-GOTO\n");
    nextToken(parser);
    parser->labelsGotoed = realloc(
        parser->labelsGotoed, sizeof(char *) * (parser->labelsGotoedCount + 1));
    parser->labelsGotoed[parser->labelsGotoedCount] =
        strdup(parser->currentToken->text);
    parser->labelsGotoedCount++;
    match(parser, IDENT_TOKEN);

  } else if (checkToken(parser, LET_TOKEN)) {
    printf("STATEMENT-LET\n");
    nextToken(parser);

    bool exists = false;
    for (int i = 0; i < parser->symbolsCount; ++i) {
      if (strcmp(parser->currentToken->text, parser->symbols[i]) == 0) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      parser->symbols =
          realloc(parser->symbols, sizeof(char *) * (parser->symbolsCount + 1));
      parser->symbols[parser->symbolsCount] =
          strdup(parser->currentToken->text);
      parser->symbolsCount++;
    }

    match(parser, IDENT_TOKEN);
    match(parser, EQ_TOKEN);
    expression(parser);

  } else if (checkToken(parser, INPUT_TOKEN)) {
    printf("STATEMENT-INPUT\n");
    nextToken(parser);

    bool exists = false;
    for (int i = 0; i < parser->symbolsCount; ++i) {
      if (strcmp(parser->currentToken->text, parser->symbols[i]) == 0) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      parser->symbols =
          realloc(parser->symbols, sizeof(char *) * (parser->symbolsCount + 1));
      parser->symbols[parser->symbolsCount] =
          strdup(parser->currentToken->text);
      parser->symbolsCount++;
    }

    match(parser, IDENT_TOKEN);

  } else {
    char error[100];
    sprintf(error, "Invalid statement at %s (%d)", parser->currentToken->text,
            parser->currentToken->kind);
    abortParser(error);
  }

  nl(parser);
}

void comparison(Parser *parser) {
  printf("COMPARISON\n");

  expression(parser);
  if (isComparisonOperator(parser)) {
    nextToken(parser);
    expression(parser);
  } else {
    abortParser("Excepted comparison operatoir");
  }

  while (isComparisonOperator(parser)) {
    nextToken(parser);
    expression(parser);
  }
}

void expression(Parser *parser) {
  printf("EXPRESSION\n");

  term(parser);
  while (checkToken(parser, PLUS_TOKEN) || checkToken(parser, MINUS_TOKEN)) {
    nextToken(parser);
    term(parser);
  }
}

void unary(Parser *parser) {
  printf("UNARY\n");

  if (checkToken(parser, PLUS_TOKEN) || checkToken(parser, MINUS_TOKEN)) {
    nextToken(parser);
  }
  primary(parser);
}

void term(Parser *parser) {
  printf("TERM\n");

  unary(parser);
  while (checkToken(parser, ASTERISK_TOKEN) ||
         checkToken(parser, SLASH_TOKEN)) {
    nextToken(parser);
    unary(parser);
  }
}

void primary(Parser *parser) {
  printf("PRIMARY (%s)\n", parser->currentToken->text);

  if (checkToken(parser, NUMBER_TOKEN)) {
    nextToken(parser);
  } else if (checkToken(parser, IDENT_TOKEN)) {
    bool exists = false;
    for (int i = 0; i < parser->symbolsCount; ++i) {
      if (strcmp(parser->currentToken->text, parser->symbols[i]) == 0) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      char error[100];
      sprintf(error, "Referencing variable before assignment: %s",
              parser->currentToken->text);
      abortParser(error);
    }
    nextToken(parser);
  } else {
    char error[100];
    sprintf(error, "Unexpected token: %s", parser->currentToken->text);
    abortParser(error);
  }
}

void nl(Parser *parser) {
  printf("NEWLINE\n");

  match(parser, NEWLINE_TOKEN);
  while (checkToken(parser, NEWLINE_TOKEN)) {
    nextToken(parser);
  }
}

void freeParser(Parser *parser) {
  if (parser->symbols) {
    for (int i = 0; i < parser->symbolsCount; ++i) {
      free(parser->symbols[i]);
    }
    free(parser->symbols);
  }

  if (parser->labelsDeclared) {
    for (int i = 0; i < parser->labelsDeclaredCount; ++i) {
      free(parser->labelsDeclared[i]);
    }
    free(parser->labelsDeclared);
  }

  if (parser->labelsGotoed) {
    for (int i = 0; i < parser->labelsGotoedCount; ++i) {
      free(parser->labelsGotoed[i]);
    }
    free(parser->labelsGotoed);
  }

  free(parser->currentToken);
  free(parser->peekToken);
}
