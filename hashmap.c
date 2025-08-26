#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// UNDER CONSTRUCTION

typedef struct int_hashmap_entry {
    char *key;
    int value;
} int_hashmap_entry;

typedef struct int_hashmap {
    int_hashmap_entry *entries;
    int cap, count;
} int_hashmap;

#define FNV_PRIME 16777619u;
#define FNV_OFFSET 2166136261u;

uint32_t
hash(char *key) {
    uint32_t hash = FNV_OFFSET;

    for(int i = 0; key[i] != 0; i++) {
        hash ^= (uint32_t)key[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

void
int_hashmap_set(int_hashmap *h, char *key, int value) {
    if(h->cap == 0) {
        h->cap = 16;
        h->count = 0;
        h->entries = malloc(h->cap * sizeof(int_hashmap_entry));
        memset(h->entries, 0, h->cap);
    } else {
        uint32_t hashed = hash(key);
        int index = hashed % h->cap;

        int probes = 0;
        while(probes < h->cap) {
            if(h->entries[index].key != NULL && strcmp(h->entries[index].key, key) == 0) {
                h->entries[index].value = value;
                return;
            }

            index = (index + 1) % h->cap;
            probes++;
        }
    }

    if(h->count >= h->cap / 2) {
        h->entries = realloc(h->entries, h->cap * 2 * sizeof(int_hashmap_entry));
        memset(h->entries + h->cap, 0, h->cap);
        h->cap *= 2;
        // todo: resize
    }

    uint32_t hashed = hash(key);
    int index = hashed % h->cap;
    while(h->entries[index].key != NULL) {
        index = (index + 1) % h->cap;
    }

    h->entries[index].key = strdup(key);
    h->entries[index].value = value;
}

typedef struct int_result {
    bool has_value;
    int value;
} int_result;

int_result
int_hashmap_get(int_hashmap *h, char *key) {
    uint32_t hashed = hash(key);
    int index = hashed % h->cap;

    int probes = 0;
    while(probes < h->cap) {
        if(h->entries[index].key != NULL && strcmp(h->entries[index].key, key) == 0) {
            return (int_result){true, h->entries[index].value};
        }

        index = (index + 1) % h->cap;
        probes++;
    }

    return (int_result){0};
}
