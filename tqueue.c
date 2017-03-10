//
// Created by Elia Oggian on 10.03.17.
//

#include <stdlib.h>
#include <stddef.h>
#include "tqueue.h"

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
    TQueueNode* result = NULL;

    for (int i = 0; i < offset % tqueue_size(q); ++i)
        result = q->next;

    return result;
}