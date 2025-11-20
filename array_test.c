#include "array.h"

#include <stdio.h>

define_array(int, int_array);
define_array(float, float_array);

struct user {
    char *name;
    char *password;
};

define_array(struct user, user_array);

int
main(int argc, char **argv) {
    int_array_t a;
    int_array_init(&a, 0, NULL);

    int_array_insert(&a, 0, 100);

    int_array_push(&a, 0);
    int_array_push(&a, 1);
    int_array_push(&a, 2);
    int_array_push(&a, 3);

    int_array_insert(&a, 2, 4);
    int_array_remove(&a, 3);

    for(int i = 0; i < a.len; i++) {
        printf("a[%d] = %d\n", i, a.data[i]);
    }

    int_array_deinit(&a);

    printf("\n");

    float_array_t f;
    float_array_init(&f, 5, (float[]){0.0, 1.0, 2.0, 3.0, 4.0});

    float_array_insert(&f, 0, 100.0);

    float_array_push(&f, 0.0);
    float_array_push(&f, 1.0);
    float_array_push(&f, 2.0);
    float_array_push(&f, 3.0);

    float_array_insert(&f, 2, 4.0);
    float_array_remove(&f, 3);

    for(int i = 0; i < f.len; i++) {
        printf("f[%d] = %.2f\n", i, f.data[i]);
    }

    float_array_deinit(&f);

    return 0;
}
