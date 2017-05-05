//
// Created by niko on 24.03.17.
//
#ifndef THREADLIBRARY_BTHREAD_PRIVATE_H_H
#define THREADLIBRARY_BTHREAD_PRIVATE_H_H

#include <setjmp.h>
#include "tqueue.h"

typedef unsigned long int bthread_t;

static int CUSHION_SIZE = 1000;


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




__bthread_scheduler_private* bthread_get_scheduler();

static void bthread_cleanup();
static void bthread_create_cushion(__bthread_private* t_data);
static void bthread_initialize_next();
static int bthread_reap_if_zombie(bthread_t bthread, void ** retval);



#define save_context(CONTEXT) sigsetjmp(CONTEXT, 1)
#define restore_context(CONTEXT) siglongjmp(CONTEXT, 1);




#endif //THREADLIBRARY_BTHREAD_PRIVATE_H_H