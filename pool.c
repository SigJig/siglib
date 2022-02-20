
#include "pool.h"
#include <string.h>

static uintptr_t
pool_alloc(struct pool* p, uintptr_t start, size_t bytes)
{
    uintptr_t headerp = start;
    uintptr_t initial = headerp;

    while (*(size_t*)headerp) {
        headerp += sizeof(size_t) + *(size_t*)headerp;

        if (headerp >= (p->mem + p->cap - sizeof(size_t))) {
            headerp = p->mem;
        }

        if (headerp == initial)
            return 0;
    }

    *(size_t*)headerp = bytes;
    p->last = headerp;

    return headerp + sizeof(size_t);
}

static uintptr_t
pool_malloc__(struct pool* p, size_t bytes)
{
    return pool_alloc(p, p->last, bytes);
}

void*
pool_malloc(struct pool* p, size_t bytes)
{
    return (void*)pool_malloc__(p, bytes);
}

static uintptr_t
pool_realloc__(struct pool* p, uintptr_t mem, size_t bytes)
{
    uintptr_t headerp = mem - sizeof(size_t);
    size_t remain = bytes - *(size_t*)headerp;

    while (remain) {
        if (*(size_t*)(mem + bytes - remain)) {
            break;
        }

        remain--;
    }

    if (!remain) {
        *(size_t*)headerp = bytes;

        return headerp + 1;
    }

    uintptr_t start_header = mem + bytes - remain;
    uintptr_t alloced = pool_alloc(
        p, start_header + sizeof(size_t) + *(size_t*)start_header, bytes);

    if (!alloced)
        return 0;

    memcpy((void*)alloced, (void*)mem, *(size_t*)headerp);

    return alloced;
}

void*
pool_realloc(struct pool* p, void* mem, size_t bytes)
{
    return (void*)pool_realloc__(p, (uintptr_t)mem, bytes);
}

void*
pool_calloc(struct pool* p, size_t nitems, size_t size)
{
    return pool_malloc(p, nitems * size);
}

void
pool_free(struct pool* p, void* mem)
{
    uintptr_t headerp = (uintptr_t)mem - sizeof(size_t);
    memset(mem, 0, sizeof(size_t) + *(size_t*)headerp);

    p->last = (uintptr_t)mem;
}