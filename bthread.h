//
// Created by niko on 24.03.17.
//
//
#include "setjmp.h"
#include "bthread_private.h"

#ifndef THREADLIBRARY_BTHREAD_H
#define THREADLIBRARY_BTHREAD_H


int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);

int bthread_join(bthread_t bthread, void **retval);

void bthread_yield();

void bthread_exit(void *retval);

int bthread_join(bthread_t bthread, void **retval);


static void bthread_create_cushion(__bthread_private* t_data);
static void bthread_initialize_next();
static int bthread_reap_if_zombie(bthread_t bthread, void **retval);
#define save_context(CONTEXT) sigsetjmp(CONTEXT, 1)
#define restore_context(CONTEXT) siglongjmp(CONTEXT, 1)



#endif //THREADLIBRARY_BTHREAD_H