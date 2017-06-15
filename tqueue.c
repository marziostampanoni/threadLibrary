//
// Created by Elia Oggian on 10.03.17.
//

#include <stdlib.h>
#include <stddef.h>
#include "tqueue.h"

unsigned long int tqueue_enqueue(TQueue* q, void* data){

    if((*q)== NULL){
        *q = (TQueueNode*)malloc(sizeof(TQueueNode*));
        (*q)->next = *q;
        (*q)->data = data;
        return 1;
    }

    unsigned long int position = 1;
    TQueue* qStart = q;


    while((*q)->next != *qStart){
        q = &((*q)->next);
        position++;
    }
    (*q)->next = (TQueueNode*)malloc(sizeof(TQueueNode*));
    (*q)->next->data = data;
    (*q)->next->next = *qStart;
    position++;

    return position;
}

void* tqueue_get_data(TQueue q){
    return q->data;
}

/*
 * Removes and returns the element at the beginning of the list,
 * NULL if the queue is empty
 */
void* tqueue_pop(TQueue* q){
    unsigned long size = tqueue_size((*q));
    if(size == 0)
        return NULL;
    else{
        TQueue temp = *q;

        if(size == 1){
            *q = NULL;
        }else {
            TQueueNode *prev = *q;

            while (prev->next != *q) {
                prev = prev->next;
            }

            prev->next = (*q)->next;
            *q = (*q)->next;
        }
        temp->next = NULL;
        return temp;
    }
}

unsigned long int tqueue_size(TQueue q){

    TQueue initial = q;

    unsigned long int size=1;

    while (q->next != initial){
        q = q->next;
        size++;
    }
    return size;
}

TQueue tqueue_at_offset(TQueue q, unsigned long int offset) {
    TQueue result = q;

    for (int i = 0; i < offset; ++i)
        result = q->next;




    return result;
}

