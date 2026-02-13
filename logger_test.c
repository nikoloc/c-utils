#define LOGGER_IMPLEMENTATION
#include "logger.h"

#include <stdlib.h>

// these are ai generated, i was lazy

void
calculate_something(int a, int b) {
    log_debug("Starting calculation with a=%d, b=%d", a, b);

    if(b == 0) {
        log_error("Division by zero attempted!");
        return;
    }

    int result = a / b;
    log_info("Calculation result: %d / %d = %d", a, b, result);
}

void
process_data(const char *data) {
    if(data == NULL) {
        log_warn("Received NULL data pointer");
        return;
    }

    log_debug("Processing data: %s", data);
    log_info("Data processed successfully");
}

int
main(void) {
    logger_init(LOG_LEVEL_DEBUG, true);

    log_info("Application started");
    log_debug("This is a debug message");
    log_info("This is an info message");
    log_warn("This is a warning message");
    log_error("This is an error message");

    calculate_something(10, 2);
    calculate_something(10, 0);

    process_data("Sample data");
    process_data(NULL);

    log_debug("This debug message won't appear");
    log_info("This info message won't appear");
    log_warn("This warning will appear");
    log_error("This error will appear");

    log_info("Application finished");

    return 0;
}
