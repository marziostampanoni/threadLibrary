//
// Created by niko on 24.03.17.
//

#include "tqueue.h"
#include "setjmp.h"
#include "bthread_private.h"

#ifndef THREADLIBRARY_BTHREAD_H
#define THREADLIBRARY_BTHREAD_H

#endif //THREADLIBRARY_BTHREAD_H


typedef struct {
    TQueue queue;
    TQueue current_item;
    jmp_buf context;
} __bthread_scheduler_private;

typedef unsigned long int bthread_t;

typedef enum { __BTHREAD_EXITED = 0, __BTHREAD_ZOMBIE, __BTHREAD_UNINITIALIZED,
    __BTHREAD_READY, __BTHREAD_BLOCKED, __BTHREAD_SLEEPING } bthread_state;


int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);

int bthread_join(bthread_t bthread, void **retval);

void bthread_yield();

void bthread_exit(void *retval);

