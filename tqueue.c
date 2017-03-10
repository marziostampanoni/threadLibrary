//
// Created by Elia Oggian on 10.03.17.
//

#include <stdlib.h>
#include "tqueue.h"

TQueue tqueue_at_offset(TQueue q, unsigned long int offset) {
    TQueueNode* result = NULL;

    for (int i = 0; i < offset % tqueue_size(q); ++i)
        result = q->next;

    return result;
}