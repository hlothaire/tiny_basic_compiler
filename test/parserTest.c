#include "../includes/lexer.h"
#include "../includes/parser.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: Compiler needs source file as argument.\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "Error: Could not open source file.\n");
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    char *source = (char *)malloc(fileSize + 1);
    fread(source, 1, fileSize, inputFile);
    source[fileSize] = '\0';

    fclose(inputFile);

    Lexer* lexer = initLexer(source);

    Parser* parser = initParser(lexer);

    program(parser);

    printf("Parsing completed.\n");

    freeParser(parser);
    free(source);

    return 0;
}
