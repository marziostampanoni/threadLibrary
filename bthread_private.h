//
// Created by niko on 24.03.17.
//
#ifndef THREADLIBRARY_BTHREAD_PRIVATE_H_H
#define THREADLIBRARY_BTHREAD_PRIVATE_H_H

#include <setjmp.h>
#include "tqueue.h"

typedef unsigned long int bthread_t;

typedef struct {
    TQueue queue;
    TQueue current_item;
    jmp_buf  context;
} __bthread_scheduler_private;

typedef struct {

} bthread_attr_t;

typedef enum {
    __BTHREAD_EXITED = 0, __BTHREAD_ZOMBIE, __BTHREAD_UNINITIALIZED,
    __BTHREAD_READY, __BTHREAD_BLOCKED, __BTHREAD_SLEEPING
} bthread_state;

typedef void *(*bthread_routine)(void *);

typedef struct {
    bthread_t tid;
    bthread_routine body;
    void *arg;
    bthread_state state;
    bthread_attr_t attr;
    jmp_buf context;
    void *retval;
}__bthread_private;


typedef struct {
    TQueue queue;
    TQueue current_item;
    jmp_buf context;
} __bthread_scheduler_private;

__bthread_scheduler_private* bthread_get_scheduler();



#endif //THREADLIBRARY_BTHREAD_PRIVATE_H_H