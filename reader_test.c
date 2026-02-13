#include <assert.h>

#include "dstring.h"

#define READER_BUFFER_SIZE 2
#define READER_IMPLEMENTATION
#include "reader.h"

int
main(void) {
    struct reader r;
    bool success = reader_init(&r, "reader_test_file.txt");
    assert(success);

    char *expect[] = {
            "first line",
            "second line",
            "",
            "fourth line",
            "",
            "",
            "",
            "some other line",
    };

    string_t s = {0};

    int i = 0;
    while(reader_read_line(&r, &s)) {
        assert(string_equal_c_string(&s, expect[i]));

        i++;
    }

    string_deinit(&s);
    reader_deinit(&r);

    return 0;
}
