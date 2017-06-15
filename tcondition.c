//
// Created by niko on 15.06.17.
//

#include <stdlib.h>
#include <assert.h>
#include "bthread_private.h"
#include "bthread.h"
#include "tcondition.h"

#define NULL 0

int bthread_cond_init(bthread_cond_t* c, const bthread_condattr_t *attr)
{
    if(c == NULL)
    {
        c = (bthread_cond_t*) malloc(sizeof(bthread_cond_t));
    }
    c->waiting_list = NULL;
}

int bthread_cond_destroy(bthread_cond_t* c)
{
    assert(tqueue_size(c->waiting_list) == 0);
    free(c);
    return 0;

}

int bthread_cond_wait(bthread_cond_t* c, bthread_mutex_t* mutex){
    bthread_block_timer_signal();

    __bthread_scheduler_private* scheduler = bthread_get_scheduler();

    __bthread_private* current_thread = tqueue_get_data(scheduler->current_item);

    if(mutex != NULL && mutex->owner == current_thread){
        __bthread_private* unlock = tqueue_pop(&mutex->waiting_list);

        if (unlock != NULL){
            mutex->owner = unlock;
            unlock->state = __BTHREAD_READY;
        }
        else
            mutex->owner = NULL;
    }

    current_thread->state = __BTHREAD_BLOCKED;
    tqueue_enqueue(&c->waiting_list, current_thread);
    while(current_thread->state != __BTHREAD_READY)
        bthread_yield();

    bthread_mutex_lock(mutex);
    bthread_unblock_timer_signal();
}

int bthread_cond_signal(bthread_cond_t* c)
{
    bthread_block_timer_signal();

    __bthread_private* thread_top_pop;

    if(tqueue_size(c->waiting_list) > 0){
        thread_top_pop = tqueue_pop(&c->waiting_list);
        thread_top_pop->state = __BTHREAD_READY;
    }

    bthread_unblock_timer_signal();
}

int bthread_cond_broadcast(bthread_cond_t* c)
{
    bthread_block_timer_signal();
    __bthread_private* thread_top_pop;

    for(int i = 0; i < tqueue_size(c->waiting_list); i++){
        thread_top_pop = tqueue_pop(&c->waiting_list);
        thread_top_pop->state = __BTHREAD_READY;
    }
    bthread_unblock_timer_signal();
}