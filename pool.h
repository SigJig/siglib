

#include <inttypes.h>
#include <stdlib.h>

typedef uint8_t byte;

struct pool {
    uintptr_t mem;
    uintptr_t last;
    size_t cap;
};

#define POOL_ARRAY(name, size) byte name##_arr[size] = {0}
#define POOL_STRUCT(name, size)                                                \
    struct pool name = {.mem = (uintptr_t)name##_arr,                          \
                        .last = (uintptr_t)name##_arr,                         \
                        .cap = size}

#define POOL_FIXED_NBYTES(nitems, size) ((size + sizeof(size_t)) * nitems)

#define POOL_FIXED(name, nitems, size)                                         \
    POOL_ARRAY(name, POOL_FIXED_NBYTES(nitems, size));                         \
    POOL_STRUCT(name, POOL_FIXED_NBYTES(nitems, size))

#define POOL_FIXED_STATIC(name, nitems, size)                                  \
    static POOL_ARRAY(name, POOL_FIXED_NBYTES(nitems, size));                  \
    static POOL_STRUCT(name, POOL_FIXED_NBYTES(nitems, size))

#define POOL(name, nbytes)                                                     \
    POOL_ARRAY(name, nbytes);                                                  \
    POOL_STRUCT(name, nbytes)

#define POOL_STATIC(name, nbytes)                                              \
    static POOL_ARRAY(name, nbytes);                                           \
    static POOL_STRUCT(name, nbytes)

void* pool_malloc(struct pool* p, size_t bytes);
void* pool_calloc(struct pool* p, size_t nitems, size_t size);
void* pool_realloc(struct pool* p, void* mem, size_t bytes);

void pool_free(struct pool* p, void* mem);
