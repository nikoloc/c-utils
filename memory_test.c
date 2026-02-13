#include "memory.h"

typedef struct test {
    int a, b;
} test_t;

int
main(void) {
    test_t *a = allocate(test_t);
    zero(a, test_t);

    test_t *many = allocate_many(3, test_t);
    assert(compare(a, a, 3, test_t));

    many = reallocate(many, 5, test_t);
    zero_many(many, 5, test_t);

    free(a);
    free(many);

    return 0;
}
