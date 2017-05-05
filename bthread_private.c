//
// Created by Elia Oggian on 28.04.17.
//

#include <stdlib.h>
#include "bthread_private.h"
#include "bthread.h"

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

void bthread_create_cushion(__bthread_private* t_data){
    char cushion[CUSHION_SIZE];
    cushion[CUSHION_SIZE-1] = cushion[0];
    t_data->state = __BTHREAD_READY;
    bthread_exit(t_data->arg);
}