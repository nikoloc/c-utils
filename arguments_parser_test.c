#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGUMENTS_PARSER_IMPLEMENTATION
#include "arguments_parser.h"

bool
custom_validator(struct arguments_parser *parser, char *value, struct arguments_parser_error *error, void *data) {
    int val = atoi(value);
    if(val < 0) {
        return false;
    }
    *(int *)data = val;
    return true;
}

void
test_basic_options() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    int32_t port = 0;
    bool verbose = false;
    char *name = NULL;

    char *argv[] = {"program", "-p", "8080", "--verbose", "-n", "gemini"};
    int argc = 6;

    arguments_parser_init(&parser, argc, argv);
    arguments_parser_add_option(&parser, "-p", "--port", ARGUMENT_TYPE_INT32, &port, NULL);
    arguments_parser_add_option(&parser, "-v", "--verbose", ARGUMENT_TYPE_BOOL, &verbose, NULL);
    arguments_parser_add_option(&parser, "-n", "--name", ARGUMENT_TYPE_STRING, &name, NULL);

    assert(arguments_parser_parse(&parser, &error) == true);
    assert(port == 8080);
    assert(verbose == true);
    assert(strcmp(name, "gemini") == 0);

    arguments_parser_deinit(&parser);
    printf("test_basic_options passed\n");
}

void
test_positional_arguments() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    char *input = NULL;
    char *output = NULL;
    uint32_t threads = 0;

    char *argv[] = {"program", "input.txt", "output.txt", "4"};
    int argc = 4;

    arguments_parser_init(&parser, argc, argv);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, &input, NULL);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, &output, NULL);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_UINT32, &threads, NULL);

    assert(arguments_parser_parse(&parser, &error) == true);
    assert(strcmp(input, "input.txt") == 0);
    assert(strcmp(output, "output.txt") == 0);
    assert(threads == 4);

    arguments_parser_deinit(&parser);
    printf("test_positional_arguments passed\n");
}

void
test_mixed_arguments() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    bool debug = false;
    char *filename = NULL;
    double scale = 0.0;

    char *argv[] = {"program", "--debug", "image.png", "-s", "1.5"};
    int argc = 5;

    arguments_parser_init(&parser, argc, argv);
    arguments_parser_add_option(&parser, "-d", "--debug", ARGUMENT_TYPE_BOOL, &debug, NULL);
    arguments_parser_add_option(&parser, "-s", "--scale", ARGUMENT_TYPE_DOUBLE, &scale, NULL);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, &filename, NULL);

    assert(arguments_parser_parse(&parser, &error) == true);
    assert(debug == true);
    assert(scale == 1.5);
    assert(strcmp(filename, "image.png") == 0);

    arguments_parser_deinit(&parser);
    printf("test_mixed_arguments passed\n");
}

void
test_custom_callback() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    int positive_val = 0;

    char *argv[] = {"program", "--val", "42"};
    int argc = 3;

    arguments_parser_init(&parser, argc, argv);
    arguments_parser_add_custom_option(&parser, NULL, "--val", custom_validator, &positive_val);

    assert(arguments_parser_parse(&parser, &error) == true);
    assert(positive_val == 42);

    arguments_parser_deinit(&parser);

    char *argv_fail[] = {"program", "--val", "-5"};
    arguments_parser_init(&parser, 3, argv_fail);
    arguments_parser_add_custom_option(&parser, NULL, "--val", custom_validator, &positive_val);

    assert(arguments_parser_parse(&parser, &error) == false);

    arguments_parser_deinit(&parser);
    printf("test_custom_callback passed\n");
}

void
test_init_from_string_basic() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    int32_t port = 0;
    bool verbose = false;
    char *name = NULL;

    arguments_parser_init_from_string(&parser, "myapp -p 9000 --verbose -n local_instance");

    assert(parser.argc == 6);
    assert(strcmp(parser.argv[0], "myapp") == 0);
    assert(strcmp(parser.argv[2], "9000") == 0);

    arguments_parser_add_option(&parser, "-p", "--port", ARGUMENT_TYPE_INT32, &port, NULL);
    arguments_parser_add_option(&parser, "-v", "--verbose", ARGUMENT_TYPE_BOOL, &verbose, NULL);
    arguments_parser_add_option(&parser, "-n", "--name", ARGUMENT_TYPE_STRING, &name, NULL);

    assert(arguments_parser_parse(&parser, &error) == true);
    assert(port == 9000);
    assert(verbose == true);
    assert(strcmp(name, "local_instance") == 0);

    arguments_parser_deinit(&parser);
    printf("test_init_from_string_basic passed\n");
}

void
test_init_from_string_quotes_and_escapes() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    char *title = NULL;
    char *path = NULL;
    char *extra = NULL;

    arguments_parser_init_from_string(&parser,
            "app --title \"The Matrix Reloaded\" --path '/usr/local/bin/space path' text\\ with\\ escapes");

    assert(parser.argc == 6);

    arguments_parser_add_option(&parser, "-t", "--title", ARGUMENT_TYPE_STRING, &title, NULL);
    arguments_parser_add_option(&parser, "-p", "--path", ARGUMENT_TYPE_STRING, &path, NULL);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, &extra, NULL);

    assert(arguments_parser_parse(&parser, &error) == true);
    assert(strcmp(title, "The Matrix Reloaded") == 0);
    assert(strcmp(path, "/usr/local/bin/space path") == 0);
    assert(strcmp(extra, "text with escapes") == 0);

    arguments_parser_deinit(&parser);
    printf("test_init_from_string_quotes_and_escapes passed\n");
}

void
test_error_handling() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    int32_t val;

    char *argv1[] = {"program", "-v"};
    arguments_parser_init(&parser, 2, argv1);
    arguments_parser_add_option(&parser, "-v", "--value", ARGUMENT_TYPE_INT32, &val, NULL);
    assert(arguments_parser_parse(&parser, &error) == false);
    assert(error.type == ARGUMENTS_PARSER_ERROR_TYPE_NO_ARGUMENT);
    arguments_parser_deinit(&parser);

    char *argv2[] = {"program", "-v", "not_a_number"};
    arguments_parser_init(&parser, 3, argv2);
    arguments_parser_add_option(&parser, "-v", "--value", ARGUMENT_TYPE_INT32, &val, NULL);
    assert(arguments_parser_parse(&parser, &error) == false);
    arguments_parser_deinit(&parser);

    char *argv3[] = {"program", "-v", "999999999999999999999999999"};
    arguments_parser_init(&parser, 3, argv3);
    arguments_parser_add_option(&parser, "-v", "--value", ARGUMENT_TYPE_INT32, &val, NULL);
    assert(arguments_parser_parse(&parser, &error) == false);
    arguments_parser_deinit(&parser);

    printf("test_error_handling passed\n");
}

void
test_is_present_flags() {
    struct arguments_parser parser;
    struct arguments_parser_error error;

    // Dest varijable
    int32_t speed = 0;
    bool force = false;
    bool cleanup = false;
    char *input_file = NULL;
    char *output_file = NULL;

    // Propratne is_present zastavice (inicijalno sve na false)
    bool speed_present = false;
    bool force_present = false;
    bool cleanup_present = false;
    bool input_present = false;
    bool output_present = false;

    // Prosleđujemo samo --speed, -f i jedan pozicioni argument (input.txt)
    // Izostavljamo --cleanup i drugi pozicioni argument
    char *argv[] = {"program", "--speed", "120", "-f", "input.txt"};
    int argc = 5;

    arguments_parser_init(&parser, argc, argv);

    // Registracija opcija sa is_present pointerima
    arguments_parser_add_option(&parser, "-s", "--speed", ARGUMENT_TYPE_INT32, &speed, &speed_present);
    arguments_parser_add_option(&parser, "-f", "--force", ARGUMENT_TYPE_BOOL, &force, &force_present);
    arguments_parser_add_option(&parser, "-c", "--cleanup", ARGUMENT_TYPE_BOOL, &cleanup, &cleanup_present);

    // Registracija pozicionih argumenata sa is_present pointerima
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, &input_file, &input_present);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, &output_file, &output_present);

    assert(arguments_parser_parse(&parser, &error) == true);

    // Evaluacija prosleđenih argumenata i njihovih is_present stanja
    assert(speed == 120);
    assert(speed_present == true);

    assert(force == true);
    assert(force_present == true);

    assert(strcmp(input_file, "input.txt") == 0);
    assert(input_present == true);

    // Evaluacija argumenata koji NISU prosleđeni u argv
    assert(cleanup == false);
    assert(cleanup_present == false);

    assert(output_file == NULL);
    assert(output_present == false);

    arguments_parser_deinit(&parser);
    printf("test_is_present_flags passed\n");
}

void
test_help_printing_extended() {
    struct arguments_parser parser;

    // Lažne varijable za setup help-a
    int32_t jobs = 1;
    bool verbose = false;
    bool version = false;
    char *config = NULL;
    int custom_data = 0;

    char *argv[] = {"telemetry-analyzer"};

    arguments_parser_init(&parser, 1, argv);

    // Konfiguracija bogatijeg skupa opcija i argumenata za reprezentativniji prikaz
    arguments_parser_add_option(&parser, "-v", "--verbose", ARGUMENT_TYPE_BOOL, &verbose, NULL);
    arguments_parser_add_option(&parser, "-V", "--version", ARGUMENT_TYPE_BOOL, &version, NULL);
    arguments_parser_add_option(&parser, "-c", "--config", ARGUMENT_TYPE_STRING, &config, NULL);
    arguments_parser_add_option(&parser, "-j", "--jobs", ARGUMENT_TYPE_INT32, &jobs, NULL);

    // Custom opcija i custom pozicioni argument
    arguments_parser_add_custom_option(&parser, "-e", "--epoch", custom_validator, &custom_data);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_STRING, NULL, NULL);
    arguments_parser_add_positional(&parser, ARGUMENT_TYPE_UINT32, NULL, NULL);
    arguments_parser_add_custom_positional(&parser, custom_validator, &custom_data);

    printf("\n--- Extended Generated Help Output Example ---\n");
    arguments_parser_print_help(&parser, stdout);
    printf("----------------------------------------------\n");

    arguments_parser_deinit(&parser);
}

int
main() {
    test_basic_options();
    test_positional_arguments();
    test_mixed_arguments();
    test_custom_callback();
    test_init_from_string_basic();
    test_init_from_string_quotes_and_escapes();
    test_error_handling();

    // Novi testovi
    test_is_present_flags();
    test_help_printing_extended();

    printf("\nall tests passed successfully!\n");
    return 0;
}
