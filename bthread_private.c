//
// Created by Elia Oggian on 28.04.17.
//

#include <stdlib.h>
#include "bthread_private.h"

__bthread_scheduler_private* bthread_get_scheduler() {
    static __bthread_scheduler_private* scheduler_private = NULL;
    if(!scheduler_private){
        scheduler_private = (__bthread_scheduler_private*) malloc(sizeof(__bthread_scheduler_private));
        scheduler_private -> queue = (TQueue) malloc(sizeof(TQueue));
    }
    return scheduler_private;
}

void bthread_cleanup(){
    __bthread_scheduler_private* scheduler =  bthread_get_scheduler();
    TQueue* queue = &(scheduler->queue);
    TQueueNode* tmp = NULL;
    while(NULL != (tmp = tqueue_pop(queue))){
        free(tmp->data);
        free(tmp);
    }
    free(scheduler);
}

static void bthread_initialize_next(){
    __bthread_scheduler_private* scheduler_private = bthread_get_scheduler();

    __bthread_private* nextThread  = (__bthread_private*) tqueue_get_data(scheduler_private->queue->next);

    if(nextThread->state == __BTHREAD_UNINITIALIZED){
        bthread_create_cushion(nextThread);
    }

}