#include <stdlib.h>

#define define_array(type, prefix)                                                          \
    typedef struct prefix##_array {                                                         \
        int len, cap;                                                                       \
        type *data;                                                                         \
    } prefix##_array;                                                                       \
                                                                                            \
    static inline void prefix##_array_push(prefix##_array *array, type elem) {              \
        if(array->cap == 0) {                                                               \
            array->cap = 16;                                                                \
            array->data = malloc(array->cap * sizeof(type));                                \
        } else if(array->cap == array->len) {                                               \
            array->cap *= 2;                                                                \
            array->data = realloc(array->data, array->cap * sizeof(type));                  \
        }                                                                                   \
                                                                                            \
        array->data[array->len] = elem;                                                     \
        array->len++;                                                                       \
    }                                                                                       \
                                                                                            \
    static inline void prefix##_array_remove(prefix##_array *array, int index) {            \
        for(int i = index; i < array->len - 1; i++)                                         \
            array->data[i] = array->data[i + 1];                                            \
                                                                                            \
        array->len--;                                                                       \
    }                                                                                       \
                                                                                            \
    static inline void prefix##_array_remove_fast(prefix##_array *array, int index) {       \
        array[index] = array[array->len - 1];                                               \
        array->len--;                                                                       \
    }                                                                                       \
                                                                                            \
    static inline void prefix##_array_insert(prefix##_array *array, int index, type elem) { \
        if(array->cap == 0) {                                                               \
            array->cap = 16;                                                                \
            array->data = malloc(array->cap * sizeof(type));                                \
        } else if(array->cap == array->len) {                                               \
            array->cap *= 2;                                                                \
            array->data = realloc(array->data, array->cap * sizeof(type));                  \
        }                                                                                   \
                                                                                            \
        for(int i = array->len - 1; i >= index; i--) {                                      \
            array->data[i + 1] = array->data[i];                                            \
        }                                                                                   \
                                                                                            \
        array->data[index] = elem;                                                          \
        array->len++;                                                                       \
    }                                                                                       \
                                                                                            \
    static inline void prefix##_array_destroy(prefix##_array *array) {                      \
        if(array->cap > 0)                                                                  \
            free(array->data);                                                              \
    }                                                                                       \
                                                                                            \
    static inline type *prefix##_array_end(prefix##_array *array) {                         \
        return &array->data[array->len];                                                    \
    }                                                                                       \
                                                                                            \
    static inline type *prefix##_array_last(prefix##_array *array) {                        \
        return &array->data[array->len - 1];                                                \
    }
