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


    free(scheduler);
}