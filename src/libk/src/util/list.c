#include "../../include/util/list.h"
#include "../../include/kmalloc.h"

typedef struct list_node
{
    void *value;
    struct list_node *prev;
    struct list_node *next;
} list_node;

struct list
{
    list_node *head;
    list_node *tail;
    size_t size;
};

static list_node *node_create(void *value)
{
    list_node *node = kmalloc(sizeof(list_node));

    if (!node)
        return NULL;

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

static list_node *node_at(list *l, int idx)
{
    if (!l)
        return NULL;

    if (idx < 0 || idx >= (int)l->size)
        return NULL;

    list_node *cur;

    if (idx < (int)(l->size / 2))
    {
        cur = l->head;

        for (int i = 0; i < idx; i++)
            cur = cur->next;
    }
    else
    {
        cur = l->tail;

        for (int i = (int)l->size - 1; i > idx; i--)
            cur = cur->prev;
    }

    return cur;
}

static void unlink_node(list *l, list_node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        l->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        l->tail = node->prev;

    l->size--;
}

list *list_create()
{
    list *l = kmalloc(sizeof(list));

    if (!l)
        return NULL;

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;

    return l;
}

void list_clear_ex(list *l, list_destroy_fn destroy)
{
    if (!l)
        return;

    list_node *cur = l->head;

    while (cur)
    {
        list_node *next = cur->next;

        if (destroy)
            destroy(cur->value);

        kfree(cur);

        cur = next;
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

void list_clear(list *l)
{
    list_clear_ex(l, NULL);
}

void list_destroy_ex(list *l, list_destroy_fn destroy)
{
    if (!l)
        return;

    list_clear_ex(l, destroy);
    kfree(l);
}

void list_destroy(list *l)
{
    list_destroy_ex(l, NULL);
}

size_t list_size(list *l)
{
    return l ? l->size : 0;
}

void *list_get(list *l, int idx)
{
    list_node *node = node_at(l, idx);

    if (!node)
        return NULL;

    return node->value;
}

int list_append(list *l, void *value)
{
    if (!l)
        return -1;

    list_node *node = node_create(value);

    if (!node)
        return -1;

    if (!l->head)
    {
        l->head = node;
        l->tail = node;
    }
    else
    {
        node->prev = l->tail;
        l->tail->next = node;
        l->tail = node;
    }

    l->size++;

    return 0;
}

int list_prepend(list *l, void *value)
{
    if (!l)
        return -1;

    list_node *node = node_create(value);

    if (!node)
        return -1;

    if (!l->head)
    {
        l->head = node;
        l->tail = node;
    }
    else
    {
        node->next = l->head;
        l->head->prev = node;
        l->head = node;
    }

    l->size++;

    return 0;
}

int list_insert(list *l, void *value, int idx)
{
    if (!l)
        return -1;

    if (idx < 0 || idx > (int)l->size)
        return -1;

    if (idx == 0)
        return list_prepend(l, value);

    if (idx == (int)l->size)
        return list_append(l, value);

    list_node *cur = node_at(l, idx);

    if (!cur)
        return -1;

    list_node *node = node_create(value);

    if (!node)
        return -1;

    node->prev = cur->prev;
    node->next = cur;

    cur->prev->next = node;
    cur->prev = node;

    l->size++;

    return 0;
}

int list_find(list *l, const void *value, list_cmp_fn cmp)
{
    if (!l || !cmp)
        return -1;

    int idx = 0;

    for (list_node *cur = l->head; cur; cur = cur->next)
    {
        if (cmp(cur->value, value) == 0)
            return idx;

        idx++;
    }

    return -1;
}

void *list_rmidx(list *l, int idx)
{
    list_node *node = node_at(l, idx);

    if (!node)
        return NULL;

    void *value = node->value;

    unlink_node(l, node);
    kfree(node);

    return value;
}

int list_rmidx_ex(list *l, int idx, list_destroy_fn destroy)
{
    list_node *node = node_at(l, idx);

    if (!node)
        return -1;

    unlink_node(l, node);

    if (destroy)
        destroy(node->value);

    kfree(node);

    return 0;
}

void *list_remove(
    list *l,
    const void *value,
    list_cmp_fn cmp)
{
    if (!l || !cmp)
        return NULL;

    int idx = list_find(l, value, cmp);

    if (idx < 0)
        return NULL;

    return list_rmidx(l, idx);
}

int list_remove_ex(list *l, const void *value, list_cmp_fn cmp, list_destroy_fn destroy)
{
    if (!l || !cmp)
        return -1;

    int idx = list_find(l, value, cmp);

    if (idx < 0)
        return -1;

    return list_rmidx_ex(l, idx, destroy);
}