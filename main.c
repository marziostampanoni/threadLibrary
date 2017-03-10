#include <stdio.h>
#include <stdlib.h>
#include "tqueue.h"


int main() {
    printf("Queue Test!\n");


    TQueue first = malloc(sizeof(TQueue));
    TQueue second = malloc(sizeof(TQueue));
    first->next = second;
    second->next = first;
    printf("The size of the given queue is %d", tqueue_size(first));

    return 0;
}