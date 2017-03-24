//
// Created by niko on 24.03.17.
//

#include "bthread.h"
#include "setjmp.h"

#ifndef THREADLIBRARY_BTHREAD_PRIVATE_H_H
#define THREADLIBRARY_BTHREAD_PRIVATE_H_H

#endif //THREADLIBRARY_BTHREAD_PRIVATE_H_H

typedef struct {
} bthread_attr_t;

typedef void *(*bthread_routine) (void *);

typedef struct {
    bthread_t tid;
    bthread_routine body;
    void* arg;
    bthread_state state;
    bthread_attr_t attr;
    jmp_buf context;
    void* retval;
} __bthread_private;

__bthread_scheduler_private* bthread_get_scheduler(); // Private

void bthread_cleanup(); // Private

