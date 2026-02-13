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
logger_set_level(enum log_level level) {
    g.level = level;
}

void
logger_set_colors(bool enable) {
    g.enable_colors = enable;
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

    if(g.enable_colors && isatty(STDERR_FILENO)) {
        fprintf(stderr, "%s%-5s%s ", colors[level], names[level], COLOR_RESET);
    } else {
        fprintf(stderr, "%-5s ", names[level]);
    }

    fprintf(stderr, "[%s:%d] ", get_filename(file), line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    fflush(stderr);
}
