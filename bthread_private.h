//
// Created by niko on 24.03.17.
//
#ifndef THREADLIBRARY_BTHREAD_PRIVATE_H_H
#define THREADLIBRARY_BTHREAD_PRIVATE_H_H

#define QUANTUM_USEC 10000



#include <setjmp.h>
#include "tqueue.h"

typedef unsigned long int bthread_t;

static int CUSHION_SIZE = 10000;

sigset_t sigsetNew;

//Signature per prodecura di scheduling
typedef void (*bthread_scheduling_routine)();


typedef struct {
    TQueue queue;
    TQueue current_item;
    jmp_buf  context;
    bthread_scheduling_routine scheduling_routine;
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
    double wake_up_time;
    int cancel_req;
    int priority;
    int credits;
}__bthread_private;



void random_scheduling();
void priority_scheduling();
__bthread_scheduler_private* bthread_get_scheduler();

static void bthread_cleanup();
static void bthread_create_cushion(__bthread_private* t_data);
int bthread_reap_if_zombie(bthread_t bthread, void ** retval);
void bthread_initialize_next();
void bthread_setup_timer();
void bthread_block_timer_signal();
void bthread_unblock_timer_signal();

#define save_context(CONTEXT) sigsetjmp(CONTEXT, 1)
#define restore_context(CONTEXT) siglongjmp(CONTEXT, 1);




#endif //THREADLIBRARY_BTHREAD_PRIVATE_H_H