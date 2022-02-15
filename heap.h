
#include <stdlib.h>

typedef struct heap_intnl* heap;

heap heap_create(size_t cap);
void heap_destroy(heap h);

void heap_push(heap h, int prio, void* value);
void* heap_pop(heap h);
void* heap_root(heap h);

void heap_print(heap h);
