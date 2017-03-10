//
// Created by Elia Oggian on 10.03.17.
//

#include <stdlib.h>
#include "tqueue.h"

unsigned long int tqueue_enqueue(TQueue* q, void* data){

    if((*q)== NULL){
        *q = (TQueueNode*)malloc(sizeof(TQueueNode*));
        (*q)->next = *q;
        (*q)->data = data;
        return 1;
    }


    unsigned long int position = 0;
    TQueue* qStart = q;


    while((*q)->next != *qStart){
        *q = (*q)->next;
        position++;
    }
    (*q)->next = (TQueueNode*)malloc(sizeof(TQueueNode*));
    (*q)->next->data = data;
    (*q)->next->next = *qStart;


    return position;
}