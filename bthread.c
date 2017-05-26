#include <stdlib.h>
#include "stdarg.h"
#include <sys/time.h>
#include <stdio.h>
#include "bthread.h"

int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg) {


    __bthread_private *thread = malloc(sizeof(__bthread_private));


    thread->arg = arg;
    thread->body = start_routine;
    thread->attr = *attr;
    thread->state = __BTHREAD_UNINITIALIZED;

    thread->tid = tqueue_enqueue(&bthread_get_scheduler()->queue, thread);

    if(bthread_get_scheduler()->current_item == NULL){
        bthread_get_scheduler()->current_item = bthread_get_scheduler()->queue;
    }

    *bthread = thread->tid;

    return 0;

}

int bthread_join(bthread_t bthread, void **retval) {
    bthread_block_timer_signal();
    bthread_setup_timer();
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    if (save_context(scheduler->context) == 0) {
        bthread_initialize_next();
        restore_context(scheduler->context);
    } else {
        __bthread_private *tp;
        do {
            if (bthread_reap_if_zombie(bthread, retval)) return 0;
            bthread_block_timer_signal();
            scheduler->current_item = tqueue_at_offset(scheduler->current_item, 1);
            tp = (__bthread_private *) tqueue_get_data(scheduler->current_item);
            if (tp->state == __BTHREAD_SLEEPING) {
                if (tp->wake_up_time <= get_current_time_millis())
                    tp->state = __BTHREAD_READY;
            }
        } while (tp->state != __BTHREAD_READY);
        bthread_unblock_timer_signal();
        restore_context(tp->context);
    }

}


void bthread_exit(void *retval) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();

    bthread_block_timer_signal();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);

    current_item->retval = retval;
    current_item->state = __BTHREAD_ZOMBIE;
    bthread_unblock_timer_signal();
    restore_context(scheduler->context);

}

double get_current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

void bthread_sleep(double ms) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();

    bthread_block_timer_signal();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);

    current_item->state = __BTHREAD_SLEEPING;
    long now = get_current_time_millis();
    current_item->wake_up_time = now + ms;

    bthread_yield();

}

void bthread_yield() {
    bthread_block_timer_signal();
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();

    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);


    if (save_context(current_item->context) == 0) {

        bthread_initialize_next();
        bthread_unblock_timer_signal();
        restore_context(scheduler->context);
    }

    bthread_unblock_timer_signal();
}

int bthread_cancel(bthread_t bthread) {
    bthread_block_timer_signal();
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    TQueue queue = scheduler->queue;
    TQueueNode *current = queue->next;

    while (current != NULL) {
        __bthread_private *current_item = tqueue_get_data(current);

        if (current_item->tid == bthread) {
            current_item->cancel_req = 1;
            bthread_unblock_timer_signal();
            return 0;
        }
    }
    bthread_unblock_timer_signal();
    return 1;
}

void bthread_testcancel() {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    bthread_block_timer_signal();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);

    int *ret_value;
    ret_value = (int *) malloc(sizeof(int));
    *ret_value = -1;

    if (current_item->cancel_req == 1)
        bthread_exit(ret_value);

    bthread_unblock_timer_signal();
}

void bthread_printf(const char *format, ...) {

    bthread_block_timer_signal();

    va_list args;
    va_start (args, format);
    vprintf(format, args);
    va_end (args);

    bthread_unblock_timer_signal();

}