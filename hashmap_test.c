#include "hashmap.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// these tests are ai generated

hashmap_define(int, intmap);
hashmap_define(double, doublemap);
hashmap_define(char *, strmap);

typedef struct {
    int x;
    int y;
} point_t;

hashmap_define(point_t, pointmap);

void
test_basic_operations() {
    printf("=== test_basic_operations ===\n");

    intmap_t map;
    intmap_init(&map);

    // test initial state
    assert(map.cap == 0);
    assert(map.count == 0);
    assert(map.entries == NULL);

    // test single insert
    intmap_set(&map, "one", 1);
    assert(map.count == 1);
    assert(map.cap >= 16);

    int val;
    assert(intmap_get(&map, "one", &val) == true);
    assert(val == 1);

    // test insert multiple
    intmap_set(&map, "two", 2);
    intmap_set(&map, "three", 3);
    assert(map.count == 3);

    assert(intmap_get(&map, "two", &val) == true);
    assert(val == 2);
    assert(intmap_get(&map, "three", &val) == true);
    assert(val == 3);

    // test update existing
    intmap_set(&map, "one", 100);
    assert(map.count == 3);  // count shouldn't increase
    assert(intmap_get(&map, "one", &val) == true);
    assert(val == 100);

    // test get nonexistent
    assert(intmap_get(&map, "nonexistent", &val) == false);

    // test contains
    assert(intmap_has(&map, "one") == true);
    assert(intmap_has(&map, "two") == true);
    assert(intmap_has(&map, "nonexistent") == false);

    // test remove
    assert(intmap_remove(&map, "two") == true);
    assert(map.count == 2);
    assert(intmap_has(&map, "two") == false);
    assert(intmap_get(&map, "two", &val) == false);

    // test remove nonexistent
    assert(intmap_remove(&map, "nonexistent") == false);
    assert(map.count == 2);

    // test remove already removed
    assert(intmap_remove(&map, "two") == false);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_empty_map() {
    printf("=== test_empty_map ===\n");

    intmap_t map;
    intmap_init(&map);

    int val;
    assert(intmap_get(&map, "anything", &val) == false);
    assert(intmap_has(&map, "anything") == false);
    assert(intmap_remove(&map, "anything") == false);
    assert(map.count == 0);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_many_insertions() {
    printf("=== test_many_insertions ===\n");

    intmap_t map;
    intmap_init(&map);

    // insert 1000 items
    for(int i = 0; i < 1000; i++) {
        char key[32];
        sprintf(key, "key_%d", i);
        intmap_set(&map, key, i * 10);
    }

    assert(map.count == 1000);

    // verify all items
    for(int i = 0; i < 1000; i++) {
        char key[32];
        sprintf(key, "key_%d", i);
        int val;
        assert(intmap_get(&map, key, &val) == true);
        assert(val == i * 10);
        assert(intmap_has(&map, key) == true);
    }

    // verify capacity grew appropriately
    assert(map.cap >= 1000);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_overwrite_values() {
    printf("=== test_overwrite_values ===\n");

    intmap_t map;
    intmap_init(&map);

    intmap_set(&map, "key", 1);
    intmap_set(&map, "key", 2);
    intmap_set(&map, "key", 3);

    assert(map.count == 1);

    int val;
    assert(intmap_get(&map, "key", &val) == true);
    assert(val == 3);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_remove_and_reinsert() {
    printf("=== test_remove_and_reinsert ===\n");

    intmap_t map;
    intmap_init(&map);

    intmap_set(&map, "key", 100);
    assert(map.count == 1);

    int val;
    assert(intmap_get(&map, "key", &val) == true);
    assert(val == 100);

    assert(intmap_remove(&map, "key") == true);
    assert(map.count == 0);
    assert(intmap_get(&map, "key", &val) == false);

    intmap_set(&map, "key", 200);
    assert(map.count == 1);
    assert(intmap_get(&map, "key", &val) == true);
    assert(val == 200);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_many_removes() {
    printf("=== test_many_removes ===\n");

    intmap_t map;
    intmap_init(&map);

    // insert 100 items
    for(int i = 0; i < 100; i++) {
        char key[32];
        sprintf(key, "key_%d", i);
        intmap_set(&map, key, i);
    }
    assert(map.count == 100);

    // remove even items
    for(int i = 0; i < 100; i += 2) {
        char key[32];
        sprintf(key, "key_%d", i);
        assert(intmap_remove(&map, key) == true);
    }
    assert(map.count == 50);

    // verify even items are gone
    for(int i = 0; i < 100; i += 2) {
        char key[32];
        sprintf(key, "key_%d", i);
        assert(intmap_has(&map, key) == false);
    }

    // verify odd items remain
    for(int i = 1; i < 100; i += 2) {
        char key[32];
        sprintf(key, "key_%d", i);
        int val;
        assert(intmap_get(&map, key, &val) == true);
        assert(val == i);
    }

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_for_each_iteration() {
    printf("=== test_for_each_iteration ===\n");

    intmap_t map;
    intmap_init(&map);

    intmap_set(&map, "a", 1);
    intmap_set(&map, "b", 2);
    intmap_set(&map, "c", 3);

    int sum = 0;
    int count = 0;

    hashmap_for_each(intmap, &map, k, v) {
        (void)k;
        sum += v;
        count++;
    }

    assert(count == 3);
    assert(sum == 6);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_for_each_empty() {
    printf("=== test_for_each_empty ===\n");

    intmap_t map;
    intmap_init(&map);

    int count = 0;
    hashmap_for_each(intmap, &map, k, v) {
        (void)k;
        (void)v;
        count++;
    }

    assert(count == 0);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_for_each_all_keys() {
    printf("=== test_for_each_all_keys ===\n");

    intmap_t map;
    intmap_init(&map);

    intmap_set(&map, "alpha", 1);
    intmap_set(&map, "beta", 2);
    intmap_set(&map, "gamma", 3);

    bool found_alpha = false;
    bool found_beta = false;
    bool found_gamma = false;

    hashmap_for_each(intmap, &map, k, v) {
        (void)v;
        if(strcmp(k, "alpha") == 0)
            found_alpha = true;
        if(strcmp(k, "beta") == 0)
            found_beta = true;
        if(strcmp(k, "gamma") == 0)
            found_gamma = true;
    }

    assert(found_alpha);
    assert(found_beta);
    assert(found_gamma);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_collision_handling() {
    printf("=== test_collision_handling ===\n");

    intmap_t map;
    intmap_init(&map);

    // insert many items to force collisions
    for(int i = 0; i < 50; i++) {
        char key[32];
        sprintf(key, "key%d", i);
        intmap_set(&map, key, i);
    }

    assert(map.count == 50);

    // verify all can be retrieved
    for(int i = 0; i < 50; i++) {
        char key[32];
        sprintf(key, "key%d", i);
        int val;
        assert(intmap_get(&map, key, &val) == true);
        assert(val == i);
    }

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_double_type() {
    printf("=== test_double_type ===\n");

    doublemap_t map;
    doublemap_init(&map);

    doublemap_set(&map, "pi", 3.14159);
    doublemap_set(&map, "e", 2.71828);

    assert(map.count == 2);

    double val;
    assert(doublemap_get(&map, "pi", &val) == true);
    assert(val > 3.14 && val < 3.15);

    assert(doublemap_get(&map, "e", &val) == true);
    assert(val > 2.71 && val < 2.72);

    doublemap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_string_type() {
    printf("=== test_string_type ===\n");

    strmap_t map;
    strmap_init(&map);

    strmap_set(&map, "greeting", "hello");
    strmap_set(&map, "farewell", "goodbye");

    assert(map.count == 2);

    char *val;
    assert(strmap_get(&map, "greeting", &val) == true);
    assert(strcmp(val, "hello") == 0);

    assert(strmap_get(&map, "farewell", &val) == true);
    assert(strcmp(val, "goodbye") == 0);

    strmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_struct_type() {
    printf("=== test_struct_type ===\n");

    pointmap_t map;
    pointmap_init(&map);

    point_t p1 = {10, 20};
    point_t p2 = {-5, 15};

    pointmap_set(&map, "p1", p1);
    pointmap_set(&map, "p2", p2);

    assert(map.count == 2);

    point_t val;
    assert(pointmap_get(&map, "p1", &val) == true);
    assert(val.x == 10);
    assert(val.y == 20);

    assert(pointmap_get(&map, "p2", &val) == true);
    assert(val.x == -5);
    assert(val.y == 15);

    pointmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_edge_case_keys() {
    printf("=== test_edge_case_keys ===\n");

    intmap_t map;
    intmap_init(&map);

    // empty string
    intmap_set(&map, "", 1);
    int val;
    assert(intmap_get(&map, "", &val) == true);
    assert(val == 1);

    // very long key
    char long_key[500];
    for(int i = 0; i < 499; i++) {
        long_key[i] = 'a' + (i % 26);
    }
    long_key[499] = '\0';

    intmap_set(&map, long_key, 2);
    assert(intmap_get(&map, long_key, &val) == true);
    assert(val == 2);

    // special characters
    intmap_set(&map, "key with spaces", 3);
    assert(intmap_get(&map, "key with spaces", &val) == true);
    assert(val == 3);

    intmap_set(&map, "key!@#$%^&*()", 4);
    assert(intmap_get(&map, "key!@#$%^&*()", &val) == true);
    assert(val == 4);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_growth_and_rehash() {
    printf("=== test_growth_and_rehash ===\n");

    intmap_t map;
    intmap_init(&map);

    size_t prev_cap = 0;
    int growth_count = 0;

    // insert items and watch capacity grow
    for(int i = 0; i < 100; i++) {
        char key[32];
        sprintf(key, "k%d", i);
        intmap_set(&map, key, i);

        if(map.cap > prev_cap) {
            growth_count++;
            prev_cap = map.cap;
        }
    }

    // should have grown at least a few times
    assert(growth_count >= 3);

    // verify all items still present after rehashing
    for(int i = 0; i < 100; i++) {
        char key[32];
        sprintf(key, "k%d", i);
        int val;
        assert(intmap_get(&map, key, &val) == true);
        assert(val == i);
    }

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_zero_values() {
    printf("=== test_zero_values ===\n");

    intmap_t map;
    intmap_init(&map);

    // test that zero values work correctly
    intmap_set(&map, "zero", 0);

    int val;
    assert(intmap_get(&map, "zero", &val) == true);
    assert(val == 0);

    assert(intmap_has(&map, "zero") == true);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_negative_values() {
    printf("=== test_negative_values ===\n");

    intmap_t map;
    intmap_init(&map);

    intmap_set(&map, "neg", -42);
    intmap_set(&map, "pos", 42);

    int val;
    assert(intmap_get(&map, "neg", &val) == true);
    assert(val == -42);

    assert(intmap_get(&map, "pos", &val) == true);
    assert(val == 42);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_deinit_clears_all() {
    printf("=== test_deinit_clears_all ===\n");

    intmap_t map;
    intmap_init(&map);

    for(int i = 0; i < 10; i++) {
        char key[32];
        sprintf(key, "key%d", i);
        intmap_set(&map, key, i);
    }

    assert(map.count == 10);
    assert(map.cap > 0);
    assert(map.entries != NULL);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

void
test_multiple_maps() {
    printf("=== test_multiple_maps ===\n");

    intmap_t map1, map2;
    intmap_init(&map1);
    intmap_init(&map2);

    intmap_set(&map1, "a", 1);
    intmap_set(&map2, "a", 2);

    int val1, val2;
    assert(intmap_get(&map1, "a", &val1) == true);
    assert(intmap_get(&map2, "a", &val2) == true);

    assert(val1 == 1);
    assert(val2 == 2);
    assert(val1 != val2);

    intmap_deinit(&map1);
    intmap_deinit(&map2);
    printf("PASSED\n\n");
}

void
test_similar_keys() {
    printf("=== test_similar_keys ===\n");

    intmap_t map;
    intmap_init(&map);

    intmap_set(&map, "test", 1);
    intmap_set(&map, "test1", 2);
    intmap_set(&map, "test2", 3);
    intmap_set(&map, "1test", 4);

    int val;
    assert(intmap_get(&map, "test", &val) == true && val == 1);
    assert(intmap_get(&map, "test1", &val) == true && val == 2);
    assert(intmap_get(&map, "test2", &val) == true && val == 3);
    assert(intmap_get(&map, "1test", &val) == true && val == 4);

    intmap_deinit(&map);
    printf("PASSED\n\n");
}

int
main() {
    printf("Running comprehensive hashmap tests...\n\n");

    test_basic_operations();
    test_empty_map();
    test_many_insertions();
    test_overwrite_values();
    test_remove_and_reinsert();
    test_many_removes();
    test_for_each_iteration();
    test_for_each_empty();
    test_for_each_all_keys();
    test_collision_handling();
    test_double_type();
    test_string_type();
    test_struct_type();
    test_edge_case_keys();
    test_growth_and_rehash();
    test_zero_values();
    test_negative_values();
    test_deinit_clears_all();
    test_multiple_maps();
    test_similar_keys();

    printf("=================================\n");
    printf("ALL TESTS PASSED! ✓\n");
    printf("=================================\n");

    return 0;
}
