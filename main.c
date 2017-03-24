#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tqueue.h"


int main() {
    printf("Queue Test!\n");


    TQueue first = malloc(sizeof(TQueue));
    TQueue second = malloc(sizeof(TQueue));
    first->next = second;
    second->next = first;
    printf("The size of the given queue is %lu\n", tqueue_size(first));

    assert(tqueue_size(first)==2);

    int test = 5;
    tqueue_enqueue(&first,&test);



    assert(tqueue_size(first)==3);

    TQueue *queue = &first;
    printf("%p", tqueue_pop(queue));

    TQueueNode *temp = first;

    printf("The size of the given queue is %lu\n", tqueue_size(*queue));

    printf("%p = %p\n", first, second);
    assert(temp == first);
    assert(tqueue_size(temp) == 1);
    assert(*queue == second);

    return 0;
}