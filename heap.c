
#include "heap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct heap_item {
    int prio;
    void* value;
};

struct heap_intnl {
    struct heap_item* mem;
    size_t len;
    size_t cap;
};

void*
p_malloc(size_t bytes)
{
    return malloc(bytes);
}

void
p_free(void* mem)
{
    free(mem);
}

struct heap_intnl*
heap_create(size_t cap)
{
    struct heap_intnl* h = p_malloc(sizeof *h);

    h->mem = p_malloc(sizeof(struct heap_item) * cap);
    h->len = 0;
    h->cap = cap;

    return h;
}

void
heap_destroy(struct heap_intnl* h)
{
    if (!h)
        return;

    p_free(h->mem);
    p_free(h);
}

static size_t
parent_idx(size_t index)
{
    return index < 3 ? 0 : index >> 1;
}

static size_t
lchild_idx(size_t index)
{
    return index ? index << 1 : 1;
}

static struct heap_item*
swap(struct heap_item* parent, struct heap_item* child)
{
    struct heap_item tmp = *child;
    *child = *parent;
    *parent = tmp;

    return parent;
}

static struct heap_item*
item_addr(struct heap_intnl* h, size_t index)
{
    return h->mem + index;
}

static bool
compare(size_t prio1, size_t prio2)
{
    return prio1 < prio2;
}

void
heap_push(struct heap_intnl* h, int prio, void* value)
{
    if (h->len >= h->cap) {
        return;
    }

    struct heap_item* item = item_addr(h, h->len);
    *item = (struct heap_item){.prio = prio, .value = value};

    size_t cur_idx = parent_idx(h->len);
    struct heap_item* par_item = item_addr(h, cur_idx);

    while (compare(item->prio, par_item->prio)) {
        item = swap(par_item, item);

        if (!cur_idx)
            break;

        cur_idx = parent_idx(cur_idx);
        par_item = item_addr(h, cur_idx);
    }

    h->len++;
}

void*
heap_pop(heap h)
{
    if (!h->len) {
        return NULL;
    }

    h->len--;

    struct heap_item* root = item_addr(h, 0);
    void* value = root->value;

    *root = *item_addr(h, h->len);

    size_t lidx = lchild_idx(0);

    while (lidx < h->len) {
        struct heap_item* lchild = item_addr(h, lidx);
        struct heap_item* rchild = lidx + 1 < h->len ? lchild + 1 : NULL;

        if (compare(root->prio, lchild->prio) &&
            (!rchild || compare(root->prio, rchild->prio)))
            break;

        bool is_right = rchild ? !compare(lchild->prio, rchild->prio) : 0;

        root = swap(root, lchild + is_right);
        lidx = lchild_idx(lidx + is_right);

        lchild = item_addr(h, lidx);
        rchild = lchild + 1;
    }

    return value;
}

void*
heap_root(heap h)
{
    if (!h->len)
        return NULL;

    struct heap_item* item = item_addr(h, 0);

    return item ? item->value : NULL;
}

void
heap_print(struct heap_intnl* h)
{
    size_t len = h->len;
    for (size_t i = 0; i < len; i++) {
        printf("%d\n", heap_pop(h));
    }
}
