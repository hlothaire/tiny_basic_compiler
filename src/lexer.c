#include "../includes/lexer.h"

Lexer *initLexer(const char *input) {
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  lexer->source = strdup(input);
  lexer->curChar = '\0';
  lexer->curPos = -1;
  nextChar(lexer);
  return lexer;
}

void freeLexer(Lexer *lexer) {
  free(lexer->source);
  free(lexer);
}

void nextChar(Lexer *lexer) {
  lexer->curPos++;
  lexer->curChar = lexer->source[lexer->curPos];
}

char peek(Lexer *lexer) { return lexer->source[lexer->curPos + 1]; }

void skipWhitespace(Lexer *lexer) {
  while (isspace(lexer->curChar)) {
    nextChar(lexer);
  }
}

void skipComment(Lexer *lexer) {
  if (lexer->curChar == '#') {
    while (lexer->curChar != '\n') {
      nextChar(lexer);
    }
  }
}

void abortLexing(Lexer *lexer, const char *message) {
  fprintf(stderr, "lexing error. %s\n", message);
  exit(EXIT_FAILURE);
}

Token *getToken(Lexer *lexer) {
  skipWhitespace(lexer);
  skipComment(lexer);
  Token *token = (Token *)malloc(sizeof(Token));
  token->text = NULL;

  switch (lexer->curChar) {
  case '+':
    token->text = "+";
    token->kind = PLUS_TOKEN;
    nextChar(lexer);
    break;
  case '-':
    token->text = "-";
    token->kind = MINUS_TOKEN;
    nextChar(lexer);
    break;
  case '*':
    token->text = "*";
    token->kind = ASTERISK_TOKEN;
    nextChar(lexer);
    break;
  case '/':
    token->text = "/";
    token->kind = SLASH_TOKEN;
    nextChar(lexer);
    break;
  case '=':
    if (peek(lexer) == '=') {
      nextChar(lexer);
      token->text = "==";
      token->kind = EQEQ_TOKEN;
    } else {
      token->text = "=";
      token->kind = EQ_TOKEN;
    }
    nextChar(lexer);
    break;
  case '>':
    if (peek(lexer) == '=') {
      nextChar(lexer);
      token->text = ">=";
      token->kind = GTEQ_TOKEN;
    } else {
      token->text = ">";
      token->kind = GT_TOKEN;
    }
    nextChar(lexer);
    break;
  case '<':
    if (peek(lexer) == '=') {
      nextChar(lexer);
      token->text = "<=";
      token->kind = LTEQ_TOKEN;
    } else {
      token->text = "<";
      token->kind = LT_TOKEN;
    }
    nextChar(lexer);
    break;
  case '!':
    if (peek(lexer) == '=') {
      nextChar(lexer);
      token->text = "!=";
      token->kind = NOTEQ_TOKEN;
    } else {
      abortLexing(lexer, "Expected !=, got !");
    }
    nextChar(lexer);
    break;
  case '\"': {
    nextChar(lexer);
    int startPos = lexer->curPos;
    while (lexer->curChar != '\"') {
      if (lexer->curChar == '\r' || lexer->curChar == '\n' ||
          lexer->curChar == '\t' || lexer->curChar == '\\' ||
          lexer->curChar == '%') {
        abortLexing(lexer, "Illegal character in string.");
      }
      nextChar(lexer);
    }
    char *tokText = strndup(lexer->source + startPos, lexer->curPos - startPos);
    token->text = tokText;
    token->kind = STRING_TOKEN;
    nextChar(lexer);
    break;
  }
  default:
    if (isdigit(lexer->curChar)) {
      int startPos = lexer->curPos;
      while (isdigit(peek(lexer))) {
        nextChar(lexer);
      }
      if (peek(lexer) == '.') {
        nextChar(lexer);
        if (!isdigit(peek(lexer))) {
          abortLexing(lexer, "Illegal character in number.");
        }
        while (isdigit(peek(lexer))) {
          nextChar(lexer);
        }
      }
      char *tokText =
          strndup(lexer->source + startPos, lexer->curPos - startPos + 1);
      token->text = tokText;
      token->kind = NUMBER_TOKEN;
      nextChar(lexer);
    } else if (isalpha(lexer->curChar)) {
      int startPos = lexer->curPos;
      while (isalnum(peek(lexer))) {
        nextChar(lexer);
      }
      char *tokText =
          strndup(lexer->source + startPos, lexer->curPos - startPos + 1);
      TokenKind keyword = checkIfKeyword(tokText);
      if (keyword == IDENT_TOKEN) {
        token->text = tokText;
        token->kind = IDENT_TOKEN;
      } else {
        token->text = tokText;
        token->kind = keyword;
      }
    } else if (lexer->curChar == '\n') {
      token->text = "\n";
      token->kind = NEWLINE_TOKEN;
      nextChar(lexer);
    } else if (lexer->curChar == '\0') {
      token->text = "";
      token->kind = EOF_TOKEN;
    } else {
      abortLexing(lexer, "Unknown token");
    }
    break;
  }
  return token;
}

void printToken(Token *token) {
  printf("Token(text='%s', type=%d)\n", token->text, token->kind);
}

TokenKind checkIfKeyword(const char *tokText) {
  if (strcmp(tokText, "LABEL") == 0)
    return LABEL_TOKEN;
  if (strcmp(tokText, "GOTO") == 0)
    return GOTO_TOKEN;
  if (strcmp(tokText, "PRINT") == 0)
    return PRINT_TOKEN;
  if (strcmp(tokText, "INPUT") == 0)
    return INPUT_TOKEN;
  if (strcmp(tokText, "LET") == 0)
    return LET_TOKEN;
  if (strcmp(tokText, "IF") == 0)
    return IF_TOKEN;
  if (strcmp(tokText, "THEN") == 0)
    return THEN_TOKEN;
  if (strcmp(tokText, "ENDIF") == 0)
    return ENDIF_TOKEN;
  if (strcmp(tokText, "WHILE") == 0)
    return WHILE_TOKEN;
  if (strcmp(tokText, "REPEAT") == 0)
    return REPEAT_TOKEN;
  if (strcmp(tokText, "ENDWHILE") == 0)
    return ENDWHILE_TOKEN;
  return IDENT_TOKEN;
}
