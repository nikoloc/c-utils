#include "memory.h"

#include "macros.h"

typedef struct test {
    int a, b;
} test_t;

int
main(void) {
    test_t *a = ALLOC(test_t);
    ZERO(a, 1, test_t);

    test_t *many = ALLOC_MANY(3, test_t);
    ASSERT(COMPARE(a, a, 3, test_t));

    many = REALLOC(many, 5, test_t);

    FREE(a);
    FREE(many);

    return 0;
}
