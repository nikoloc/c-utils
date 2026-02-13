#ifndef MEMORY_H
#define MEMORY_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define copy(dest, src, count, type) (memcpy(dest, src, count * sizeof(type)))
#define move(dest, src, count, type) (memmove(dest, src, count * sizeof(type)))
#define compare(a, b, count, type) (memcmp(a, b, count * sizeof(type)) == 0)
#define zero(dest, count, type) (memset(dest, 0, count * sizeof(type)))

static inline void *
_allocate(size_t count, size_t size) {
    void *ptr = calloc(count, size);
    assert(ptr);

    return ptr;
}

static inline void *
_reallocate(void *ptr, size_t count, size_t size) {
    ptr = realloc(ptr, count * size);
    assert(ptr);

    return ptr;
}

#define allocate(type) ((type *)_allocate(1, sizeof(type)))
#define allocate_many(count, type) ((type *)_allocate(count, sizeof(type)))
#define reallocate(ptr, count, type) ((type *)_reallocate(ptr, count, sizeof(type)))
// for convinience i also have it by this name, more suitable to use when we are not sure
// wheather the pointer we have already has some memory allocated, or not
#define reserve(ptr, count, type) ((type *)_reallocate(ptr, count, sizeof(type)))

#endif
