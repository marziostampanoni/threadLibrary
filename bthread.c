#include <stdlib.h>
#include "bthread.h";
#include "bthread_private.h"

int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg){


    __bthread_private *thread = malloc(sizeof(__bthread_private));

    thread->tid = *bthread;
    thread->arg = arg;
    thread->body = start_routine;
    thread->attr = *attr;
    thread->state = __BTHREAD_UNINITIALIZED;

    TQueue element = malloc(sizeof(TQueue));

    element->data =thread;

    tqueue_enqueue(&bthread_get_scheduler()->queue,element);

    return 0;

}