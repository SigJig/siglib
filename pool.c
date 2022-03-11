
#include "pool.h"
#include <assert.h>
#include <string.h>

static uintptr_t
h_last_possible(struct pool* p)
{
    return p->mem + p->cap - HEADER_SIZE - p->size;
}

static uintptr_t
pool_alloc(struct pool* p, uintptr_t start)
{
    uintptr_t headerp = start;
    uintptr_t initial = headerp;

    while (*(uint8_t*)headerp) {
        headerp += HEADER_SIZE + p->size;

        if (headerp > h_last_possible(p)) {
            headerp = p->mem;
        }

        if (headerp == initial)
            return 0;
    }

    *(uint8_t*)headerp = 1;
    p->last = headerp;

    return headerp + HEADER_SIZE;
}

void*
pool_reserve(struct pool* p)
{
    return (void*)pool_alloc(p, p->last);
}

static void
pool_free__(struct pool* p, uintptr_t mem)
{
    if (!(mem >= p->mem + HEADER_SIZE &&
          mem <= h_last_possible(p) + HEADER_SIZE)) {
        assert(0 /* Attempted pool_free on invalid memory location */);

        return;
    }

    uintptr_t headerp = mem - HEADER_SIZE;
    *(uint8_t*)headerp = 0;
    p->last = headerp;
}

void
pool_free(struct pool* p, void* mem)
{
    pool_free__(p, (uintptr_t)mem);
}