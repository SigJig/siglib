

#include <inttypes.h>
#include <stdlib.h>

typedef uint8_t byte;
typedef uint8_t header_type;

#define HEADER_SIZE (sizeof(header_type))

struct pool {
    uintptr_t mem;
    uintptr_t last;
    size_t size;
    size_t cap;
};

#define POOL_ARRAY(name, cap) byte name##_arr[cap] = {0}
#define POOL_STRUCT(name, cap_, size_)                                         \
    struct pool name = {.mem = (uintptr_t)name##_arr,                          \
                        .last = (uintptr_t)name##_arr,                         \
                        .size = size_,                                         \
                        .cap = cap_}

#define POOL_FIXED_NBYTES(nitems, size) ((size + HEADER_SIZE) * nitems)

#define POOL_FIXED(name, nitems, size)                                         \
    POOL_ARRAY(name, POOL_FIXED_NBYTES(nitems, size));                         \
    POOL_STRUCT(name, POOL_FIXED_NBYTES(nitems, size), size)

#define POOL_FIXED_STATIC(name, nitems, size)                                  \
    static POOL_ARRAY(name, POOL_FIXED_NBYTES(nitems, size));                  \
    static POOL_STRUCT(name, POOL_FIXED_NBYTES(nitems, size), size)

void* pool_reserve(struct pool* p);
void pool_free(struct pool* p, void* mem);
