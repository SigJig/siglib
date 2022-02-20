
#include "heap.h"
#include "pool.h"
#include <assert.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define AMOUNT 10000
#define MEASUREMENTS 100

struct ll_item {
    int prio;
    void* data;
    struct ll_item* next;
    struct ll_item* prev;
};

struct ll {
    struct ll_item* front;
};

#define USE_POOL 1

#if USE_POOL
POOL_FIXED_STATIC(mempool, AMOUNT * 10, sizeof(struct ll_item));
#endif

static void*
p_malloc(size_t nbytes)
{
#if USE_POOL
    return pool_malloc(&mempool, nbytes);
#else
    return malloc(nbytes);
#endif
}

static void
p_free(void* mem)
{
#if USE_POOL
    pool_free(&mempool, mem);
#else
    free(mem);
#endif
}

static struct ll_item*
make_item(int prio, void* data)
{
    struct ll_item* item = p_malloc(sizeof *item);

    if (!item)
        return NULL;

    item->prio = prio;
    item->data = data;
    item->next = NULL;
    item->prev = NULL;

    return item;
}

static void
free_item(struct ll_item* item)
{
    p_free(item);
}

struct ll
ll_new()
{
    return (struct ll){.front = NULL};
}

void
ll_free(struct ll* list)
{
    struct ll_item* cur = list->front;
    struct ll_item* tmp = NULL;

    while (cur) {
        tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

void
ll_insert_sorted(struct ll* list, int prio, void* data)
{
    struct ll_item* new = make_item(prio, data);

    if (!list->front) {
        list->front = new;

        return;
    }

    struct ll_item* cur = list->front;
    bool head = true;

    while (cur->prio < prio) {
        head = false;

        if (!cur->next)
            break;

        cur = cur->next;
    }

    if (head) {
        list->front = new;
        new->next = cur;
        cur->prev = new;
    } else {
        if (cur->next)
            cur->next->prev = new;

        new->prev = cur;
        new->next = cur->next;
        cur->next = new;
    }
}

int
ll_pop(struct ll* list)
{
    struct ll_item* front = list->front;
    void* value = front->data;
    int x = front->prio;

    if (front->next) {
        front->next->prev = NULL;
    }

    list->front = front->next;

    free_item(front);

    return x;
}

void
ll_print(struct ll* list)
{
    struct ll_item* cur = list->front;

    while (cur) {
        printf("%d\n", cur->prio);
        cur = cur->next;
    }
}

static double
tdiff(double t)
{
    return ((double)(clock() - t)) / 1000000;
}

static double
sum(double* arr, size_t len)
{
    double s = 0;
    for (size_t i = 0; i < len; i++) {
        s += arr[i];
    }

    return s;
}

static double
tmax(double* arr, size_t len)
{
    double max = -1;

    for (size_t i = 0; i < len; i++) {
        if (arr[i] > max)
            max = arr[i];
    }

    return max;
}

static double
tmin(double* arr, size_t len)
{
    double min = DBL_MAX;

    for (size_t i = 0; i < len; i++) {
        if (arr[i] < min)
            min = arr[i];
    }

    return min;
}

typedef void (*addfn)(void*, int, void*);
typedef void* (*rmfn)(void*);
typedef void* (*iterfn)(void*);

static void
print_time(const char* name, double* arr)
{
    double s = sum(arr, MEASUREMENTS);
    printf("\t%s: total(%lf), max(%lf), min(%lf), avg(%lf)\n", name, s,
           tmax(arr, MEASUREMENTS), tmin(arr, MEASUREMENTS), s / MEASUREMENTS);
}

static void
time_it(void* itm, addfn add, rmfn rm)
{
    double adds[MEASUREMENTS];
    double rms[MEASUREMENTS];

    for (size_t i = 0; i < MEASUREMENTS; i++) {
        clock_t t = clock();
        for (int j = 0; j < AMOUNT; j++) {
            add(itm, j, NULL);
        }
        adds[i] = tdiff(t);

        t = clock();
        for (int j = 0; j < AMOUNT; j++) {
            rm(itm);
        }
        rms[i] = tdiff(t);
    }

    print_time("adds", adds);
    print_time("remove", rms);
}

int
main(void)
{
    /*
    void* last = NULL;
    for (size_t i = 0; i < 10000; i++) {
        void* p = pool_malloc(&mempool, sizeof(struct ll_item));

        if (last && p != last + sizeof(struct ll_item) + sizeof(size_t)) {
            assert(false);
        }

        last = p;
    }*/

    heap h = heap_create(AMOUNT);
#if 0
    for (size_t i = 0; i < 100; i++) {
        heap_push(h, i, NULL);
    }

    for (size_t i = 0; i < 100; i++) {
        heap_pop(h);
    }

    heap_destroy(h);
    return 0;

#endif

    printf("heap\n");
    time_it(h, (addfn)heap_push, (rmfn)heap_pop);

    heap_destroy(h);
    struct ll list = ll_new();
    /*
        ll_insert_sorted(&list, 2, NULL);
        ll_insert_sorted(&list, 1, NULL);
        ll_insert_sorted(&list, 3, NULL);
        ll_insert_sorted(&list, 6, NULL);
        ll_insert_sorted(&list, 18, NULL);

        ll_print(&list);

        for (int i = 0; i < 5; i++) {
            printf("%d\n", ll_pop(&list));
        }*/

    printf("linked list\n");
    time_it(&list, (addfn)ll_insert_sorted, (rmfn)ll_pop);

    ll_free(&list);

    return 0;
}
