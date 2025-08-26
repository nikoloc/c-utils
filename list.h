#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct list {
    struct list *prev;
    struct list *next;
} list;

static inline void
list_init(list *list) {
    list->prev = list;
    list->next = list;
}

static inline void
list_insert(list *l, list *elm) {
    elm->prev = l;
    elm->next = l->next;
    l->next = elm;
    elm->next->prev = elm;
}

static inline void
list_remove(list *elm) {
    elm->prev->next = elm->next;
    elm->next->prev = elm->prev;
    elm->next = NULL;
    elm->prev = NULL;
}

static inline int
list_length(list *l) {
    struct list *e;
    int count;

    count = 0;
    e = l->next;
    while(e != l) {
        e = e->next;
        count++;
    }

    return count;
}

static inline bool
list_is_empty(list *l) {
    return l->next == l;
}

static inline bool
list_is_first(list *l, list *elm) {
    return l->prev == elm;
}

static inline bool
list_is_last(list *l, list *elm) {
    return l->next == elm;
}

static inline void
list_insert_list(list *l, list *other) {
    if(list_is_empty(other))
        return;

    other->next->prev = l;
    other->prev->next = l->next;
    l->next->prev = other->prev;
    l->next = other->next;
}

#define container_of(ptr, sample, member) (typeof(sample))((char *)(ptr) - offsetof(typeof(*sample), member))

#define list_for_each(pos, head, member)                                       \
    for(pos = container_of((head)->next, pos, member); &pos->member != (head); \
            pos = container_of(pos->member.next, pos, member))

#define list_for_each_safe(pos, tmp, head, member)                                                          \
    for(pos = container_of((head)->next, pos, member), tmp = container_of((pos)->member.next, tmp, member); \
            &pos->member != (head); pos = tmp, tmp = container_of(pos->member.next, tmp, member))

#define list_for_each_reverse(pos, head, member)                               \
    for(pos = container_of((head)->prev, pos, member); &pos->member != (head); \
            pos = container_of(pos->member.prev, pos, member))

#define list_for_each_reverse_safe(pos, tmp, head, member)                                                  \
    for(pos = container_of((head)->prev, pos, member), tmp = container_of((pos)->member.prev, tmp, member); \
            &pos->member != (head); pos = tmp, tmp = container_of(pos->member.prev, tmp, member))

#endif
