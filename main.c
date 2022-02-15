
#include "heap.h"
#include <stdbool.h>

int
main(void)
{
    heap h = heap_create(10);
    heap_push(h, 2, NULL);
    heap_push(h, 1, NULL);
    heap_push(h, 3, NULL);
    heap_push(h, 6, NULL);
    heap_push(h, 18, NULL);

    heap_print(h);

    heap_destroy(h);

    return 0;
}
