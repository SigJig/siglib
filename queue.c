
#include "queue.h"

struct q_item {
    void* data;
    struct q_item* next;
};

struct q_intnl {
    struct q_item* front;
    struct q_item* back;
};

struct q_intnl*
q_create()
{
}

void
q_destroy(struct q_intnl* q)
{
}

void
q_push(struct q_intnl* q, void* data)
{
}

void*
q_pop(struct q_intnl* q)
{
}