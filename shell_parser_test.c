#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SHELL_PARSER_IMPLEMENTATION
#include "shell_parser.h"

// ai generated

void
test_basic_tokenization() {
    struct shell_parser parser;
    char buffer[256];

    shell_parser_init(&parser, "echo hello world");

    // Prvi token
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "echo") == 0);

    // Drugi token
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "hello") == 0);

    // Treci token
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "world") == 0);

    // Kraj stringa
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    printf("test_basic_tokenization passed\n");
}

void
test_excessive_whitespaces() {
    struct shell_parser parser;
    char buffer[256];

    // Višestruki razmaci, tabulatori i novi redovi između tokena
    shell_parser_init(&parser, "  \t  cmd \n\r   arg1  \t arg2   ");

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "cmd") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "arg1") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "arg2") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    printf("test_excessive_whitespaces passed\n");
}

void
test_single_and_double_quotes() {
    struct shell_parser parser;
    char buffer[256];

    shell_parser_init(&parser, "mkdir \"New Folder\" 'Secret Documents' normal_arg");

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "mkdir") == 0);

    // Dvostruki navodnici čuvaju razmake, ali sami navodnici se brišu
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "New Folder") == 0);

    // Jednostruki navodnici takođe
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "Secret Documents") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "normal_arg") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    printf("test_single_and_double_quotes passed\n");
}

void
test_escaped_characters() {
    struct shell_parser parser;
    char buffer[256];

    // Testiranje escape karaktera van i unutar navodnika
    shell_parser_init(&parser, "file\\ name.txt \"escaped \\\" quote\" \"double\\\\slash\"");

    // Van navodnika: \space postaje običan razmak koji ne deli token
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "file name.txt") == 0);

    // Unutar dvostrukih navodnika: \" postaje samo "
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "escaped \" quote") == 0);

    // Unutar dvostrukih navodnika: \\ postaje samo
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "double\\slash") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    printf("test_escaped_characters passed\n");
}

void
test_mixed_quotes_and_attachments() {
    struct shell_parser parser;
    char buffer[256];

    // Tekst zalepljen direktno uz navodnike (npr. putanje ili konkatenacija)
    shell_parser_init(&parser, "executable=\"/usr/bin/git\" --prefix=/home/'user name'/bin");

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "executable=/usr/bin/git") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "--prefix=/home/user name/bin") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    printf("test_mixed_quotes_and_attachments passed\n");
}

void
test_buffer_overflow_protection() {
    struct shell_parser parser;
    // Mali bafer da namerno izazovemo i proverimo zaštitu od prekoračenja
    char small_buffer[6];

    shell_parser_init(&parser, "very_long_argument short");

    // "very_long_argument" ima 18 karaktera. Bafer je veličine 6.
    // Očekujemo da upiše "very_" (5 karaktera) i terminirajuću nulu '\0' (6. karakter).
    assert(shell_parser_pop(&parser, sizeof(small_buffer), small_buffer) == true);
    assert(strcmp(small_buffer, "very_") == 0);

    // Parser je morao bezbedno da pomeri pokazivač do kraja tog tokena,
    // tako da sledeći poziv uredno vuče naredni token.
    assert(shell_parser_pop(&parser, sizeof(small_buffer), small_buffer) == true);
    assert(strcmp(small_buffer, "short") == 0);

    assert(shell_parser_pop(&parser, sizeof(small_buffer), small_buffer) == false);

    printf("test_buffer_overflow_protection passed\n");
}

void
test_empty_and_edge_inputs() {
    struct shell_parser parser;
    char buffer[256];

    // 1. Prazan string
    shell_parser_init(&parser, "");
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    // 2. String koji sadrži samo razmake
    shell_parser_init(&parser, "    \t\n  ");
    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == false);

    // 3. Prazni navodnici kao legitimni argumenti
    shell_parser_init(&parser, "echo \"\" '' after_empty");

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "echo") == 0);

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "") == 0);  // Prazan string iz ""

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "") == 0);  // Prazan string iz ''

    assert(shell_parser_pop(&parser, sizeof(buffer), buffer) == true);
    assert(strcmp(buffer, "after_empty") == 0);

    printf("test_empty_and_edge_inputs passed\n");
}

int
main() {
    test_basic_tokenization();
    test_excessive_whitespaces();
    test_single_and_double_quotes();
    test_escaped_characters();
    test_mixed_quotes_and_attachments();
    test_buffer_overflow_protection();
    test_empty_and_edge_inputs();

    printf("\nAll shell parser tests passed successfully!\n");
    return 0;
}
