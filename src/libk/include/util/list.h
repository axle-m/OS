#pragma once

#include "../ktypes.h"

typedef struct list list;

typedef int (*list_cmp_fn)(const void *a, const void *b);
typedef void (*list_destroy_fn)(void *value);

// creates empty list
list *list_create();

// frees all nodes but leaves list allocated
void list_clear(list *l);

// frees all nodes and values using destroy
void list_clear_ex(list *l, list_destroy_fn destroy);

// frees all nodes and the list
void list_destroy(list *l);

// frees all nodes, values, and the list
void list_destroy_ex(list *l, list_destroy_fn destroy);

// returns number of elements
size_t list_size(list *l);

// returns value at idx or NULL if invalid
void *list_get(list *l, int idx);

// appends value to list
int list_append(list *l, void *value);

// prepends value to list
int list_prepend(list *l, void *value);

// inserts value at idx
int list_insert(list *l, void *value, int idx);

// returns index of value using cmp, -1 if not found
int list_find(list *l, const void *value, list_cmp_fn cmp);

// removes first matching value using cmp
void *list_remove(list *l, const void *value, list_cmp_fn cmp);

// removes and destroys first matching value
int list_remove_ex(
    list *l,
    const void *value,
    list_cmp_fn cmp,
    list_destroy_fn destroy);

// removes value at idx
void *list_rmidx(list *l, int idx);

// removes and destroys value at idx
int list_rmidx_ex(
    list *l,
    int idx,
    list_destroy_fn destroy);