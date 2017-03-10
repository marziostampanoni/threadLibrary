//
// Created by Elia Oggian on 10.03.17.
//

#include "tqueue.h"

/* Removes and returns the element at the beginning of the list, NULL if the
queue is empty */
void* tqueue_pop(TQueue* q){
    if(tqueue_size(q) == 0)
        return NULL;
    TQueueNode* temp = q;
    if(q != q->next)
        q = q->next;
    else{
        q = NULL;
    }
    return temp;
}