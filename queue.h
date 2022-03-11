
#ifndef QUEUE_H
#define QUEUE_H

typedef struct q_intnl* queue;

queue q_create();
void q_destroy(queue);

void q_push(queue, void*);
void* q_pop(queue);

#endif // QUEUE_H