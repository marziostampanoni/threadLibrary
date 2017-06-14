//
// Created by Elia Oggian on 28.04.17.
//

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>
#include "bthread_private.h"
#include "bthread.h"
#include <sys/time.h>
#include "stdarg.h"

void random_scheduling() {
    // Faccio restituire lo scheduler
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    //Estraggo la coda di thread
    TQueue* thread_queue = scheduler->queue;
    //Random scheduling
    scheduler->current_item = tqueue_at_offset(&thread_queue, rand() % tqueue_size(&thread_queue));
}

void priority_scheduling() {
    // Faccio restituire lo scheduler
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    // Estraggo la coda di thread
    TQueue* thread_queue = scheduler->queue;
    if (((__bthread_private*)tqueue_get_data(scheduler->current_item))->credits == 0) {
        scheduler->current_item = tqueue_at_offset(&thread_queue, rand() % tqueue_size(&thread_queue));
        ((__bthread_private*)tqueue_get_data(scheduler->current_item))->credits = ((__bthread_private*)tqueue_get_data(scheduler->current_item))->priority;
    } else {
        //printf("Thread %d credits %d\n", scheduler->current_item->bthread->tid, scheduler->current_item->bthread->credits);
        ((__bthread_private*)tqueue_get_data(scheduler->current_item))->credits--;
    }
}

__bthread_scheduler_private *bthread_get_scheduler() {
    static __bthread_scheduler_private *scheduler_private = NULL;
    if (!scheduler_private) {
        scheduler_private = (__bthread_scheduler_private *) malloc(sizeof(__bthread_scheduler_private));
    }
    //scheduler_private->scheduling_routine = random_scheduling;
    //scheduler_private->scheduling_routine = priority_scheduling;
    return scheduler_private;
}

void bthread_cleanup() {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    TQueue *queue = &(scheduler->queue);
    TQueueNode *tmp = NULL;
    while (NULL != (tmp = tqueue_pop(queue))) {
        free(tmp->data);
        free(tmp);
    }
    free(scheduler);
}

void bthread_create_cushion(__bthread_private *t_data) {
    char cushion[CUSHION_SIZE];
    cushion[CUSHION_SIZE - 1] = cushion[0];
    t_data->state = __BTHREAD_READY;
    bthread_exit(t_data->body(t_data->arg));
    bthread_unblock_timer_signal();

}

int bthread_reap_if_zombie(bthread_t bthread, void **retval) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    bthread_block_timer_signal();
    __bthread_private *thread = tqueue_get_data(scheduler->current_item);

    if (thread->state == __BTHREAD_ZOMBIE && thread->tid == bthread){

        thread->state = __BTHREAD_EXITED;
        if (thread->retval != NULL)
            *retval = thread->retval;

        bthread_unblock_timer_signal();
        return 1;
    }
    else {

        bthread_unblock_timer_signal();
        return 0;
    }
}

void bthread_initialize_next() {
    __bthread_scheduler_private *scheduler_private = bthread_get_scheduler();

    bthread_block_timer_signal();
    __bthread_private *nextThread = tqueue_get_data(scheduler_private->current_item->next);


    if (nextThread->state == __BTHREAD_UNINITIALIZED) {
        scheduler_private->current_item = scheduler_private->current_item->next;
        bthread_create_cushion(nextThread);
    }

    bthread_unblock_timer_signal();
}
 void bthread_setup_timer() {

    static bool initialized = false;

    if (!initialized) {

        signal(SIGVTALRM, (void (*)()) bthread_yield);
        sigaddset(&sigsetNew, SIGVTALRM);
        struct itimerval time;
        time.it_interval.tv_sec = 0;
        time.it_interval.tv_usec = QUANTUM_USEC;
        time.it_value.tv_sec = 0;
        time.it_value.tv_usec = QUANTUM_USEC;
        initialized = true;
        setitimer(ITIMER_VIRTUAL, &time, NULL);
    }
}

void bthread_block_timer_signal(){
    sigprocmask(SIG_BLOCK, &sigsetNew, NULL);
}
void bthread_unblock_timer_signal(){
    sigprocmask(SIG_UNBLOCK, &sigsetNew, NULL);
}