#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include <stdbool.h>
#include <stdio.h>

enum argument_type {
    ARGUMENT_TYPE_BOOL,
    ARGUMENT_TYPE_INT32,
    ARGUMENT_TYPE_UINT32,
    ARGUMENT_TYPE_DOUBLE,
    ARGUMENT_TYPE_STRING,
};

struct arguments_parser;
struct arguments_parser_error;
typedef bool (*arguments_parser_custom_callback_t)(struct arguments_parser *parser, char *value,
        struct arguments_parser_error *error, void *data);

struct positional {
    bool *is_present;

    bool is_custom;
    union {
        struct {
            enum argument_type type;
            void *dest;
        };
        struct {
            arguments_parser_custom_callback_t callback;
            void *data;
        };
    };

    struct positional *next;
};

struct option {
    char *_short, *_long;
    bool *is_present;

    bool is_custom;
    union {
        struct {
            enum argument_type type;
            void *dest;
        };
        struct {
            arguments_parser_custom_callback_t callback;
            void *data;
        };
    };

    struct option *next;
};

struct arguments_parser {
    int argc;
    char **argv;

    struct positional *pos_head, *pos_tail;
    struct option *opt_head;

    char *_buffer;
};

enum arguments_parser_error_type {
    ARGUMENTS_PARSER_ERROR_TYPE_INVALID_TYPE = 0,
    ARGUMENTS_PARSER_ERROR_TYPE_NO_ARGUMENT,
};

struct arguments_parser_error {
    enum arguments_parser_error_type type;

    // aditional info
    char *key;
    enum argument_type expected;
};

void
arguments_parser_init(struct arguments_parser *parser, int argc, char **argv);

void
arguments_parser_init_from_string(struct arguments_parser *parser, char *str);

void
arguments_parser_print_help(struct arguments_parser *parser, FILE *stream);

void
arguments_parser_add_positional(struct arguments_parser *parser, enum argument_type type, void *dest, bool *is_present);

void
arguments_parser_add_option(struct arguments_parser *parser, char *_short, char *_long, enum argument_type type,
        void *dest, bool *is_present);

void
arguments_parser_add_custom_positional(struct arguments_parser *parser, arguments_parser_custom_callback_t callback,
        void *data);

void
arguments_parser_add_custom_option(struct arguments_parser *parser, char *_short, char *_long,
        arguments_parser_custom_callback_t callback, void *data);

bool
arguments_parser_parse(struct arguments_parser *parser, struct arguments_parser_error *error);

void
arguments_parser_deinit(struct arguments_parser *parser);

#endif

#ifdef ARGUMENTS_PARSER_IMPLEMENTATION

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool
_parse(enum argument_type type, char *key, void *dest, struct arguments_parser_error *error) {
    switch(type) {
        case ARGUMENT_TYPE_INT32: {
            char *end;
            errno = 0;
            long result = strtol(key, &end, 0);

            if(*end != 0 || errno == ERANGE) {
                error->type = ARGUMENTS_PARSER_ERROR_TYPE_INVALID_TYPE;
                error->key = key;
                error->expected = type;
                return false;
            }

            *(int32_t *)dest = (int32_t)result;
            break;
        }
        case ARGUMENT_TYPE_UINT32: {
            char *end;
            errno = 0;
            unsigned long result = strtoul(key, &end, 0);

            if(*end != 0 || errno == ERANGE) {
                error->type = ARGUMENTS_PARSER_ERROR_TYPE_INVALID_TYPE;
                error->key = key;
                error->expected = type;
                return false;
            }

            *(uint32_t *)dest = (uint32_t)result;
            break;
        }
        case ARGUMENT_TYPE_DOUBLE: {
            char *end;
            errno = 0;
            double result = strtod(key, &end);

            if(*end != 0 || errno == ERANGE) {
                error->type = ARGUMENTS_PARSER_ERROR_TYPE_INVALID_TYPE;
                error->key = key;
                error->expected = type;
                return false;
            }

            *(double *)dest = result;
            break;
        }
        case ARGUMENT_TYPE_STRING: {
            *(char **)dest = key;
            break;
        }
        default: {
            return false;
        }
    }

    return true;
}

void
arguments_parser_init(struct arguments_parser *parser, int argc, char **argv) {
    memset(parser, 0, sizeof(*parser));
    parser->argc = argc;
    parser->argv = argv;
}

void
arguments_parser_init_from_string(struct arguments_parser *parser, char *str) {
    memset(parser, 0, sizeof(*parser));

    size_t len = strlen(str);
    parser->_buffer = malloc(len + 1);
    if(!parser->_buffer) {
        return;
    }

    // estimation allocation for argv array
    size_t max_args = (len / 2) + 2;
    parser->argv = malloc(max_args * sizeof(char *));
    if(!parser->argv) {
        free(parser->_buffer);
        parser->_buffer = NULL;
        return;
    }

    char *src = str;
    char *dst = parser->_buffer;
    int argc = 0;

    while(*src) {
        // skip leading whitespace
        while(*src == ' ' || *src == '\t' || *src == '\n' || *src == '\r') {
            src++;
        }

        if(*src == '\0') {
            break;
        }

        // record the start of a token
        parser->argv[argc++] = dst;

        char in_quote = 0;
        while(*src) {
            if(in_quote) {
                if(*src == in_quote) {
                    in_quote = 0;
                    src++;
                } else if(*src == '\\' && in_quote == '"' && (*(src + 1) == '"' || *(src + 1) == '\\')) {
                    // handle escaped chars inside double quotes
                    src++;
                    *dst++ = *src++;
                } else {
                    *dst++ = *src++;
                }
            } else {
                if(*src == ' ' || *src == '\t' || *src == '\n' || *src == '\r') {
                    src++;
                    break;
                } else if(*src == '\'' || *src == '"') {
                    in_quote = *src;
                    src++;
                } else if(*src == '\\' && *(src + 1) != '\0') {
                    src++;
                    *dst++ = *src++;
                } else {
                    *dst++ = *src++;
                }
            }
        }
        *dst++ = '\0';
    }

    parser->argc = argc;
}

static const char *
_get_type_name(enum argument_type type) {
    switch(type) {
        case ARGUMENT_TYPE_BOOL:
            return "";
        case ARGUMENT_TYPE_INT32:
            return "<int32>";
        case ARGUMENT_TYPE_UINT32:
            return "<uint32>";
        case ARGUMENT_TYPE_DOUBLE:
            return "<double>";
        case ARGUMENT_TYPE_STRING:
            return "<string>";
        default:
            return "<unknown>";
    }
}

void
arguments_parser_print_help(struct arguments_parser *parser, FILE *stream) {
    const char *prog = parser->argc > 0 ? parser->argv[0] : "program";

    // print usage banner
    fprintf(stream, "usage: %s", prog);
    if(parser->opt_head) {
        fprintf(stream, " [options]");
    }

    struct positional *pos = parser->pos_head;
    int pos_count = 1;
    while(pos) {
        if(pos->is_custom) {
            fprintf(stream, " <arg%d>", pos_count);
        } else {
            fprintf(stream, " %s", _get_type_name(pos->type));
        }
        pos_count++;
        pos = pos->next;
    }
    fprintf(stream, "\n\n");

    // print options section if any exist
    if(parser->opt_head) {
        fprintf(stream, "options:\n");
        struct option *opt = parser->opt_head;
        while(opt) {
            fprintf(stream, "\t");

            if(opt->_short) {
                fprintf(stream, "%s", opt->_short);
            }

            if(opt->_short && opt->_long) {
                fprintf(stream, ", ");
            }

            if(opt->_long) {
                fprintf(stream, "%s", opt->_long);
            }

            if(!opt->is_custom) {
                fprintf(stream, " %s", _get_type_name(opt->type));
            } else {
                fprintf(stream, " <value>");
            }

            fprintf(stream, "\n");
            opt = opt->next;
        }
    }
}

void
arguments_parser_add_positional(struct arguments_parser *parser, enum argument_type type, void *dest,
        bool *is_present) {
    struct positional *pos = calloc(1, sizeof(struct positional));
    pos->is_custom = false;
    pos->type = type;
    pos->dest = dest;

    pos->is_present = is_present;

    if(!parser->pos_head) {
        parser->pos_head = pos;
    } else {
        parser->pos_tail->next = pos;
    }

    parser->pos_tail = pos;
}

void
arguments_parser_add_option(struct arguments_parser *parser, char *_short, char *_long, enum argument_type type,
        void *dest, bool *is_present) {
    struct option *opt = calloc(1, sizeof(struct option));
    opt->is_custom = false;
    opt->type = type;
    opt->dest = dest;
    opt->_short = _short;
    opt->_long = _long;

    opt->is_present = is_present;

    opt->next = parser->opt_head;
    parser->opt_head = opt;
}

void
arguments_parser_add_custom_positional(struct arguments_parser *parser, arguments_parser_custom_callback_t callback,
        void *data) {
    struct positional *pos = calloc(1, sizeof(struct positional));
    pos->is_custom = true;
    pos->callback = callback;
    pos->data = data;

    if(!parser->pos_head) {
        parser->pos_head = pos;
    } else {
        parser->pos_tail->next = pos;
    }

    parser->pos_tail = pos;
}

void
arguments_parser_add_custom_option(struct arguments_parser *parser, char *_short, char *_long,
        arguments_parser_custom_callback_t callback, void *data) {
    struct option *opt = calloc(1, sizeof(struct option));
    opt->is_custom = true;
    opt->callback = callback;
    opt->data = data;
    opt->_short = _short;
    opt->_long = _long;

    opt->next = parser->opt_head;
    parser->opt_head = opt;
}

static inline bool
_equal(char *a, char *b) {
    if(!a || !b) {
        return false;
    }

    return strcmp(a, b) == 0;
}

bool
arguments_parser_parse(struct arguments_parser *parser, struct arguments_parser_error *error) {
    struct positional *curr_pos = parser->pos_head;

    for(size_t i = 1; i < parser->argc; i++) {
        struct option *opt = parser->opt_head;
        bool found_opt = false;

        while(opt) {
            if(_equal(opt->_short, parser->argv[i]) || _equal(opt->_long, parser->argv[i])) {
                if(opt->type == ARGUMENT_TYPE_BOOL) {
                    *((bool *)opt->dest) = true;
                    if(opt->is_present) {
                        *opt->is_present = true;
                    }
                    found_opt = true;
                    break;
                }

                if(i == parser->argc - 1) {
                    // if this is the last argument and its not boolean
                    error->type = ARGUMENTS_PARSER_ERROR_TYPE_NO_ARGUMENT;
                    error->key = parser->argv[i];
                    return false;
                }

                if(opt->is_custom) {
                    if(!opt->callback(parser, parser->argv[i + 1], error, opt->data)) {
                        return false;
                    }
                } else {
                    if(!_parse(opt->type, parser->argv[i + 1], opt->dest, error)) {
                        return false;
                    }
                }

                // skip the next key because its consumed as an argument for this option
                i++;
                found_opt = true;
                if(opt->is_present) {
                    *opt->is_present = true;
                }
                break;
            }

            opt = opt->next;
        }

        if(!found_opt) {
            if(curr_pos) {
                if(curr_pos->is_custom) {
                    if(!curr_pos->callback(parser, parser->argv[i], error, curr_pos->data)) {
                        return false;
                    }
                } else {
                    if(!_parse(curr_pos->type, parser->argv[i], curr_pos->dest, error)) {
                        return false;
                    }
                }
                if(curr_pos->is_present) {
                    *curr_pos->is_present = true;
                }
                curr_pos = curr_pos->next;
            } else {
                // unknown
                error->key = parser->argv[i];
                return false;
            }
        }
    }

    return true;
}

void
arguments_parser_deinit(struct arguments_parser *parser) {
    struct option *opt = parser->opt_head;
    while(opt) {
        struct option *tmp = opt;
        opt = opt->next;
        free(tmp);
    }

    struct positional *pos = parser->pos_head;
    while(pos) {
        struct positional *tmp = pos;
        pos = pos->next;
        free(tmp);
    }

    if(parser->_buffer) {
        free(parser->_buffer);
        free(parser->argv);
    }

    memset(parser, 0, sizeof(*parser));
}

#endif
