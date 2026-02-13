#include "array.h"
#define DSTRING_DEFAULT_CAP 4
#define DSTRING_IMPLEMENTATION
#include "dstring.h"

int
main(void) {
    string_t s;
    string_init(&s, NULL);

    char *test = "this is a testing string";
    for(char *p = test; *p; p++) {
        string_append(&s, *p);
    }

    assert(string_equal_c_string(&s, "this is a testing string"));
    assert(s.len == 24);
    assert(strlen(string_c_string_view(&s)) == 24);

    string_append_c_string(&s, ", and another one");
    assert(string_equal_c_string(&s, "this is a testing string, and another one"));

    // 0-initialized struct is a valid string, so you dont have to call `string_init()` on calloced struct members for
    // example
    string_t clone = {0};
    string_clone(&clone, &s);
    assert(string_equal(&clone, &s));
    assert(clone.len == s.len);
    string_deinit(&clone);

    string_t clone2;
    // we can clone even in a already used string
    string_init(&clone2, "this is going to be overriden");
    string_clone(&clone2, &s);
    assert(string_equal(&clone2, &s));
    assert(clone2.len == s.len);
    string_deinit(&clone2);

    string_t substring = {0};
    string_init(&substring, "this substring has a initial value");
    string_substring(&substring, &s, 10, 17);
    assert(string_equal_c_string(&substring, "testing"));
    string_deinit(&substring);

    string_assign(&s, "new value for this string");
    assert(string_equal_c_string(&s, "new value for this string"));
    assert(s.len == 25);

    // todo: fix this example
    int test_index = 0;

    string_assign(&s, "find , and . and the last , and also the period.");
    int first_comma = string_index_of(&s, ',');
    assert(first_comma == 5);

    int last_comma = string_index_of_reverse(&s, ',');
    assert(last_comma == 26);

    int test_count = 0;
    int expect[] = {5, 11, 26, 47};

    char sep;
    int period_comma = -1;
    while((period_comma = string_index_of_any_from(&s, ".,", &sep, period_comma + 1)) != -1) {
        assert(period_comma == expect[test_count]);
        test_count++;
    }

    string_deinit(&s);

    string_t parse;
    string_init(&parse, "parse just on spaces now");
    char *expect1[] = {"parse", "just", "on", "spaces", "now"};

    string_array_t arr = {0};
    string_split(&parse, ' ', false, &arr);
    assert(arr.len == 5);
    for(int i = 0; i < 5; i++) {
        assert(string_equal_c_string(&arr.data[i], expect1[i]));
        string_deinit(&arr.data[i]);
    }

    string_assign(&parse, "parse      just      on spaces        now but       with      multiple     ");
    char *expect2[] = {"parse", "just", "on", "spaces", "now", "but", "with", "multiple"};

    string_split(&parse, ' ', true, &arr);
    assert(arr.len == 8);
    for(int i = 0; i < 8; i++) {
        assert(string_equal_c_string(&arr.data[i], expect2[i]));
    }

    // todo: fix the example
    // string_assign(&parse, "parse shell      like \"string     with spaces\"      and \"esca\\\"ping\"");
    // string_shell_split(&parse, &arr);
    // for(string_t *iter = arr.data; iter < string_array_end(&arr); iter++) {
    //     printf("`%s`\n", string_c_string_view(iter));
    //     string_deinit(iter);
    // }

    string_split_destroy_all(&arr);
    string_array_deinit(&arr);
    string_deinit(&parse);

    return 0;
}
