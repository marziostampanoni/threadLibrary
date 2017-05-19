#include <stdlib.h>
#include <sys/time.h>
#include "bthread.h";
#include "bthread_private.h"
//commento

int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg) {


    __bthread_private *thread = malloc(sizeof(__bthread_private));

    thread->tid = *bthread;
    thread->arg = arg;
    thread->body = start_routine;
    thread->attr = *attr;
    thread->state = __BTHREAD_UNINITIALIZED;

    TQueue element = malloc(sizeof(TQueue));

    element->data = thread;

    tqueue_enqueue(&bthread_get_scheduler()->queue, element);

    return 0;

}

int bthread_join(bthread_t bthread, void **retval) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    if (save_context(scheduler->context) == 0) {
        bthread_initialize_next();
        restore_context(scheduler->context);
    } else {
        __bthread_private *tp;
        do {
            if (bthread_reap_if_zombie(bthread, retval)) return 0;
            scheduler->current_item = tqueue_at_offset(scheduler->current_item, 1);
            tp = (__bthread_private *) tqueue_get_data(scheduler->current_item);
            if(tp->state == __BTHREAD_SLEEPING){
                if(tp->wake_up_time>=get_current_time_millis())
                    tp->state = __BTHREAD_READY;
            }
        } while (tp->state != __BTHREAD_READY);
        restore_context(tp->context);
    }
}


void bthread_exit(void *retval) {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);

    current_item->retval = retval;
    current_item->state = __BTHREAD_ZOMBIE;
    restore_context(scheduler->context);

}

double get_current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

void bthread_sleep(double ms){
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);

    current_item->state = __BTHREAD_SLEEPING;

    current_item->wake_up_time = get_current_time_millis() + ms;

    bthread_yield();

}

void bthread_yield() {
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);


    if (save_context(current_item->context) == 0) { //salvo lo stato

        bthread_initialize_next(); //si occupa lui di passare il testimone allo scheduler e di controllare se fare il cuscino
        restore_context(scheduler->context);
    }

}

int bthread_cancel(bthread_t bthread){
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    TQueue queue = scheduler->queue;
    TQueueNode *current = queue->next;

    while(current != NULL){
        __bthread_private *current_item = tqueue_get_data(current);

        if(current_item->tid == bthread){
            current_item->cancel_req = 1;
            return 0;
        }
    }
    return 1;
}

void bthread_testcancel(){
    __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    __bthread_private *current_item = tqueue_get_data(scheduler->current_item);

    int *ret_value;
    ret_value = (int *)malloc(sizeof(int));
    *ret_value = -1;

    if(current_item->cancel_req == 1)
        bthread_exit(ret_value);
}
