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
    assert(*queue == first);

    printf("%p\n", tqueue_pop(queue));
    assert(*queue == second);

    TQueueNode *temp = tqueue_pop(queue);

    printf("The size of the given queue is %lu\n", tqueue_size(*queue));

    printf("%p = %p = %p \n", first, second, temp);
    assert(temp == second);
    assert(tqueue_size(*queue) == 1);
    tqueue_pop(queue);
    assert(*queue == NULL);

    return 0;
}