#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

enum log_level {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3,
};

void
logger_init(enum log_level level, bool enable_colors);

void
logger_log(enum log_level level, char *file, int line, char *fmt, ...);

#define log_debug(...) logger_log(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) logger_log(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) logger_log(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) logger_log(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif

#ifdef LOGGER_IMPLEMENTATION

#include <stdarg.h>
#include <string.h>

#define COLOR_RESET "\x1b[0m"
#define COLOR_DEBUG "\x1b[36m"  // Cyan
#define COLOR_INFO "\x1b[32m"  // Green
#define COLOR_WARN "\x1b[33m"  // Yellow
#define COLOR_ERROR "\x1b[31m"  // Red

// global state
static struct g {
    enum log_level level;
    bool enable_colors;
} g;

static const char *names[] = {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
};

static const char *colors[] = {
        COLOR_DEBUG,
        COLOR_INFO,
        COLOR_WARN,
        COLOR_ERROR,
};

void
logger_init(enum log_level level, bool enable_colors) {
    g.level = level;
    g.enable_colors = enable_colors;
}

static char *
get_filename(char *path) {
    char *filename = strrchr(path, '/');
    if(filename) {
        return filename + 1;
    }
    filename = strrchr(path, '\\');
    if(filename) {
        return filename + 1;
    }

    return path;
}

void
logger_log(enum log_level level, char *file, int line, char *fmt, ...) {
    if(level < g.level) {
        return;
    }

    fprintf(stderr, "[%s:%d] ", get_filename(file), line);

    if(g.enable_colors && isatty(STDERR_FILENO)) {
        fprintf(stderr, "%s%s%s ", colors[level], names[level], COLOR_RESET);
    } else {
        fprintf(stderr, "%s ", names[level]);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    fflush(stderr);
}

#endif
