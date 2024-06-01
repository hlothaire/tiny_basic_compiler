#include "../includes/lexer.h"

Lexer* initLexer(const char* input) {
	Lexer* lexer = (Lexer *)malloc(sizeof(Lexer));
	lexer->source = strdup(input);
	lexer->curChar = '\0';
	lexer->curPos = -1;
	nextChar(lexer);
	return lexer;
}

void freeLexer(Lexer* lexer) {
	free(lexer->source);
	free(lexer);
}

void nextChar(Lexer *lexer) {
	lexer->curPos++;
	lexer->curChar = lexer->source[lexer->curPos];
}

char peek(Lexer* lexer) {
	return lexer->source[lexer->curPos+1];
}

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
            token->type = PLUS_TOKEN;
            nextChar(lexer);
            break;
        case '-':
            token->text = "-";
            token->type = MINUS_TOKEN;
            nextChar(lexer);
            break;
	// others cases
        default:
            abortLexing(lexer, "Unknown token");
    }

    return token;
}


void printToken(Token *token) {
	printf("Token(text='%s', type=%d)\n",token->text,token->type);
}
