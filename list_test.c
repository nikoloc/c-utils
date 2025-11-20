#include "list.h"

#include <stdio.h>
#include <stdlib.h>

struct test {
    int a, b;
    list_t link;
};

int
main(void) {
    list_t list;
    list_init(&list);

    struct test *tenth = NULL;

    for(int i = 0; i < 100; i++) {
        struct test *test = calloc(1, sizeof(*test));
        test->a = i;
        test->b = 2 * i;

        if(i == 10)
            tenth = test;

        list_insert(&list, &test->link);
    }

    for(int i = 0; i < 100; i += 2) {
        struct test *test = calloc(1, sizeof(*test));
        test->a = -i;
        test->b = -2 * i;

        list_insert(&tenth->link, &test->link);
    }

    list_for_each(struct test, iter, &list, link) {
        printf("%d, %d\n", iter->a, iter->b);
    }

    printf("list len = %d\n", list_length(&list));

    int i = 0;
    list_for_each_safe(struct test, iter, &list, link) {
        if(i % 3 == 0) {
            list_remove(&iter->link);
            free(iter);
        }

        i++;
    }

    printf("-------------------\n");

    list_for_each_reverse(struct test, iter, &list, link) {
        printf("%d, %d\n", iter->a, iter->b);
    }

    printf("list len = %d\n", list_length(&list));

    list_for_each_reverse_safe(struct test, iter, &list, link) {
        free(iter);
    }

    return 0;
}
