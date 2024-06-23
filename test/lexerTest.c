#include "../includes/lexer.h"

int main() {
  char *source = "IF+12 foo*THEN/";
  Lexer *lexer = initLexer(source);
  Token *token = NULL;

  do {
    token = getToken(lexer);
    printToken(token);
  } while (token->kind != EOF_TOKEN);

  freeLexer(lexer);
  return 0;
}
