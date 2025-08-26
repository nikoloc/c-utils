#ifndef DYN_STRING_H
#define DYN_STRING_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef STRING_DEFAULT_CAP
#define STRING_DEFAULT_CAP 16
#endif

typedef struct string {
    int len, cap;
    char *data;
} string;

static inline void
string_reserve(string *s, int cap) {
    if(cap < STRING_DEFAULT_CAP)
        cap = STRING_DEFAULT_CAP;

    if(cap <= s->cap)
        return;

    s->cap = cap;
    s->data = realloc(s->data, cap * sizeof(char));
}

static inline void
string_init(string *s, char *v) {
    int len;
    if(v == NULL || (len = strlen(v)) == 0) {
        memset(s, 0, sizeof(*s));
        return;
    }

    s->len = len;
    string_reserve(s, len);

    memcpy(s->data, v, s->len);
}

static inline void
string_deinit(string *s) {
    if(s->cap > 0)
        free(s->data);
}

static inline void
string_assign(string *s, char *v) {
    s->len = strlen(v);
    string_reserve(s, s->len);
    memcpy(s->data, v, s->len);
}

static inline void
string_clone(string *dest, string *src) {
    if(src->len == 0) {
        string_init(dest, NULL);
        return;
    }

    dest->len = src->len;
    string_reserve(dest, dest->len);
    memcpy(dest->data, src->data, src->len);
}

static inline bool
string_equal(string *a, string *b) {
    if(a->len != b->len)
        return false;

    return strcmp(a->data, b->data) == 0;
}

static inline int
string_index_of_from(string *s, char c, int start) {
    for(int i = start; i < s->len; i++)
        if(s->data[i] == c)
            return i;

    return -1;
}

static inline int
string_index_of(string *s, char c) {
    return string_index_of_from(s, c, 0);
}

static inline int
string_index_of_from_reverse(string *s, char c, int start) {
    for(int i = start; i >= 0; i--)
        if(s->data[i] == c)
            return i;

    return -1;
}

static inline int
string_index_of_reverse(string *s, char c) {
    return string_index_of_from_reverse(s, c, s->len - 1);
}

static inline int
string_index_of_any_from(string *s, char *c, char *which, int start) {
    for(int i = start; i < s->len; i++) {
        for(char *t = c; *t != 0; t++) {
            if(s->data[i] == *t) {
                if(which != NULL)
                    *which = *t;

                return i;
            }
        }
    }

    return -1;
}

static inline int
string_index_of_any(string *s, char *c, char *which) {
    return string_index_of_any_from(s, c, which, 0);
}

static inline void
string_substring(string *dest, string *src, int start, int end) {
    int len = end - start;

    if(len <= 0) {
        string_init(dest, NULL);
        return;
    }

    dest->len = len;
    string_reserve(dest, len);
    memcpy(dest->data, src->data + start, len);
}

static inline void
string_append(string *s, char c) {
    if(s->len == s->cap)
        string_reserve(s, 2 * s->cap);

    s->data[s->len] = c;
    s->len++;
}

static inline void
string_append_string(string *s, string *a) {
    int new_len = s->len + a->len;
    if(new_len > s->cap)
        string_reserve(s, new_len);

    memcpy(s->data + s->len, a->data, a->len);
    s->len = new_len;
}

static inline void
string_append_c_string(string *s, char *a) {
    for(char *p = a; *p != 0; p++)
        string_append(s, *p);
}

static inline char *
string_c_string_view(string *s) {
    if(s->len == s->cap)
        string_reserve(s, s->cap + 1);

    s->data[s->len] = 0;
    return s->data;
}

#endif
