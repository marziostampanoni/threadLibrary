//
// Created by Elia Oggian on 10.03.17.
//

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
