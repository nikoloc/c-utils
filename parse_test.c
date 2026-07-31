#include "parse.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

// ai generated

void
test_parse_int_success() {
    int result = 0;

    // Bazični pozitivni i negativni brojevi
    assert(parse_int("42", &result) == true);
    assert(result == 42);

    assert(parse_int("-1337", &result) == true);
    assert(result == -1337);

    assert(parse_int("0", &result) == true);
    assert(result == 0);

    // Heksadekadni i oktalni unosi (strtol sa bazom 0 ovo automatski podržava)
    assert(parse_int("0x10", &result) == true);
    assert(result == 16);

    assert(parse_int("010", &result) == true);
    assert(result == 8);

    // Brojevi sa belinama (whitespace) na početku
    assert(parse_int("   500", &result) == true);
    assert(result == 500);

    // Granične vrednosti za običan int (INT_MIN i INT_MAX)
    char buf[64];

    snprintf(buf, sizeof(buf), "%d", INT_MAX);
    assert(parse_int(buf, &result) == true);
    assert(result == INT_MAX);

    snprintf(buf, sizeof(buf), "%d", INT_MIN);
    assert(parse_int(buf, &result) == true);
    assert(result == INT_MIN);

    printf("test_parse_int_success passed\n");
}

void
test_parse_int_failure() {
    int result = 0;

    // Čist tekst
    assert(parse_int("abc", &result) == false);
    assert(parse_int("42abc", &result) == false);  // Delimičan tekst na kraju

    // Prazni stringovi i beline
    assert(parse_int("", &result) == false);
    assert(parse_int("   ", &result) == false);

    // NULL pokazivač
    assert(parse_int(NULL, &result) == false);

    // Overflow i underflow provere (izvan INT_MAX i INT_MIN)
    // Koristimo stringove koji su garantovano van opsega 32-bitnog int-a
    assert(parse_int("2147483648", &result) == false);  // INT_MAX + 1
    assert(parse_int("-2147483649", &result) == false);  // INT_MIN - 1
    assert(parse_int("99999999999999999999", &result) == false);  // Ekstremni overflow

    printf("test_parse_int_failure passed\n");
}

void
test_parse_float_success() {
    float result = 0.0f;

    // Bazični float unosi
    assert(parse_float("3.14", &result) == true);
    assert(result >= 3.139f && result <= 3.141f);  // Float preciznost tolerancija

    assert(parse_float("-0.005", &result) == true);
    assert(result == -0.005f);

    assert(parse_float("0", &result) == true);
    assert(result == 0.0f);

    // Naučna notacija (eksponenti)
    assert(parse_float("1e3", &result) == true);
    assert(result == 1000.0f);

    assert(parse_float("2.5e-2", &result) == true);
    assert(result == 0.025f);

    // Beline na početku
    assert(parse_float("   -42.42", &result) == true);
    assert(result == -42.42f);

    printf("test_parse_float_success passed\n");
}

void
test_parse_float_failure() {
    float result = 0.0f;

    // Nevalidni tekstualni unosi
    assert(parse_float("not_a_float", &result) == false);
    assert(parse_float("1.23xyz", &result) == false);

    // Prazni stringovi i beline
    assert(parse_float("", &result) == false);
    assert(parse_float("   ", &result) == false);

    // NULL pokazivač
    assert(parse_float(NULL, &result) == false);

    // Ekstremni overflow za float opseg
    assert(parse_float("1e39", &result) == false);
    assert(parse_float("-1e39", &result) == false);

    printf("test_parse_float_failure passed\n");
}

int
main() {
    test_parse_int_success();
    test_parse_int_failure();
    test_parse_float_success();
    test_parse_float_failure();

    printf("\nAll parser tests passed successfully!\n");
    return 0;
}
