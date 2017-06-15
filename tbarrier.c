//
// Created by niko on 15.06.17.
//

#include <stdlib.h>
#include <assert.h>
#include "tbarrier.h"
#include "bthread_private.h"
#include "bthread.h"

int bthread_barrier_init(bthread_barrier_t* b, const bthread_barrierattr_t* attr, unsigned count){
    if(b == NULL)
        b = (bthread_barrier_t*) malloc(sizeof(bthread_barrier_t));

    b->bsize = count;
    b->waiting_list = NULL;
    b->count = 0;

    return 0;
}

int bthread_barrier_destroy(bthread_barrier_t* b){
    assert(tqueue_size(b->waiting_list) == 0);
    free(b);
    return 0;
}

int bthread_barrier_wait(bthread_barrier_t* b){
    bthread_block_timer_signal();

    __bthread_private* current_thread =tqueue_get_data(bthread_get_scheduler()->current_item);

    b->count++;
    __bthread_private* t;

    if(b->count == b->bsize){
        b->count = 0;
        while((t = tqueue_pop(&b->waiting_list)) != NULL)
            t->state = __BTHREAD_READY;

    }
    else{
        current_thread->state = __BTHREAD_BLOCKED;
        tqueue_enqueue(&b->waiting_list, current_thread);
        bthread_yield();
    }
    bthread_unblock_timer_signal();
    return 0;
}