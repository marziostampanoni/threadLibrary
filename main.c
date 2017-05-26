#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "bthread.h"

#define N 3

void* dormire(void* value){
    int* i;
    i = (int*) value;
    bthread_printf("Io ezzere thread figlio von thread %d padre, io dorme 5 zecondi\n",*i);
    bthread_sleep(5000);
    bthread_printf("Thread figlio von thread %d padre finito",*i);
}

void* run(void* value){
    int* i;
    i = (int*) value;
    bthread_printf("Io ezzere thread %d, io dorme %d zecondi\n",*i, 2+1* *i);

    bthread_sleep(2000 + 1000* *i);


    if(*i==10){
        bthread_printf("Io ezzere thread %d, io fatto figlio. Figlio dormire\n",*i);
        bthread_t newThread;

        bthread_create(&newThread,NULL,dormire,i);
        bthread_join(newThread,NULL);
    }

    bthread_printf("Thread %d ezzere terminato ja\n",*i);

}



int main(){

    bthread_t threads[N];
    int values[N];
    for(int i=0;i<N;i++){
        values[i] = i;
        bthread_create(&threads[i],NULL,run,&values[i]);
    }

    for(int i=0;i<N;i++){
        bthread_join(threads[i], NULL);
    }

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