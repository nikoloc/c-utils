#include "dynamic_string.h"

#include <stdio.h>

int
main(void) {
    string s;
    string_init(&s, NULL);

    char *test = "this is a testing string";
    for(char *p = test; *p != 0; p++)
        string_append(&s, *p);

    printf("string after copying = `%s`\n", string_c_string_view(&s));

    string_append_c_string(&s, ", and another one");
    printf("string after appending = `%s`\n", string_c_string_view(&s));

    // 0-initialized struct is a valid string, so you dont have to call `string_init()` on calloced struct members for
    // example
    string clone = {0};
    string_clone(&clone, &s);
    printf("cloned string = `%s`\n", string_c_string_view(&clone));
    string_deinit(&clone);

    string clone2;
    // we can clone even in a already used string
    string_init(&clone2, "this is going to be overriden");
    string_clone(&clone2, &s);
    printf("cloned inited string = `%s`\n", string_c_string_view(&clone2));
    string_deinit(&clone2);

    string substring = {0};
    string_init(&substring, "this substring has a initial value");
    string_substring(&substring, &s, 10, 17);
    printf("substring(10, 17) = `%s`\n", string_c_string_view(&substring));
    string_deinit(&substring);

    string_assign(&s, "new value for this string");
    printf("after assignment = `%s`\n", string_c_string_view(&s));

    string_deinit(&s);

    string parse;
    string_init(&parse, "parse on spaces,commas and/front slash");

    char sep;
    int index = -1;
    while(index != parse.len) {
        int next = string_index_of_any_from(&parse, " ,/", &sep, index + 1);
        if(next == -1)
            next = parse.len;

        string part = {0};
        string_substring(&part, &parse, index + 1, next);
        printf("`%s`, sep=%c\n", string_c_string_view(&part), sep);
        string_deinit(&part);

        index = next;
    }

    string_deinit(&parse);
}
