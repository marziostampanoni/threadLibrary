//
// Created by niko on 15.06.17.
//

#include <stdlib.h>
#include <assert.h>
#include "tsemaphore.h"

#define NULL 0

int bthread_sem_init(bthread_sem_t* m, int pshared, int value){
    if(m == NULL)
        m = (bthread_sem_t*) malloc(sizeof(bthread_sem_t));

    m->value = value;
    m->waiting_list = NULL;
}

int bthread_sem_destroy(bthread_sem_t* m){
    assert(tqueue_size(m->waiting_list) == 0);
    free(m);
    return 0;
}

int bthread_sem_wait(bthread_sem_t* m){
    bthread_block_timer_signal();
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    __bthread_private* current_thread = tqueue_get_data(scheduler->current_item);

    if(m->value > 0) {
        m->value--;
        bthread_unblock_timer_signal();
    }
    else {
        current_thread->state = __BTHREAD_BLOCKED;
        tqueue_enqueue(&m->waiting_list, current_thread);
        while(current_thread->state != __BTHREAD_READY){
            bthread_yield();
        }
    }

    return 0;
}

int bthread_sem_post(bthread_sem_t* m){
    bthread_block_timer_signal();
    m->value++;
    TQueue queue = m->waiting_list;
    __bthread_private* ready_thread;

    if(tqueue_size(queue) > 0){
        ready_thread = tqueue_pop(&queue);
        ready_thread->state = __BTHREAD_READY;
        m->value--;
        bthread_yield();
    }
    bthread_unblock_timer_signal();
    return 0;
}