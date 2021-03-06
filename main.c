#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "bthread.h"
#include "bthread_test.h"

#define N 3

void* run(void* value){
    int* i;
    long counter;
    i = (int*) value;


    bthread_printf("Io ezzere thread %d, io dorme %d zecondi\n",*i, 2+1* *i);

    for (int x = 0; x < 1000000; ++x) {
        bthread_printf("Sono il thread %d\n", *i);
        counter++;
    }
    //bthread_sleep(100);

    bthread_printf("Thread %d terminato, counter : %d \n",*i,counter);

}

bthread_t threads[N];


int main(){

/*
    int values[N];
    for(int i=0;i<N;i++){
        values[i] = i;
        bthread_create(&threads[i],NULL,run,&values[i]);
    }

    for(int i=0;i<N;i++){
        bthread_join(threads[i], NULL);
    }
*/

    thread_test();


}

//OLD MAIN
/* int main() {
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
}*/