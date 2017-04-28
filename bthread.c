#include "bthread.h";


int bthread_join(bthread_t bthread, void **retval){
    __bthread_scheduler_private* scheduler = bthread_get_scheduler();
    if (save_context(scheduler->context) == 0) {
        bthread_initialize_next();
        restore_context(scheduler->context);
    } else {
        __bthread_private* tp;
        do {
            if (bthread_reap_if_zombie(bthread, retval)) return 0;
            scheduler->current_item = tqueue_at_offset(scheduler->current_item, 1);
            tp = (__bthread_private*) tqueue_get_data(scheduler->current_item);
        } while (tp->state != __BTHREAD_READY);
        restore_context(tp->context);
    }
}


