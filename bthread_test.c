//
// Created by Elia Oggian on 15.06.17.
//

#include "bthread_test.h"
#include "bthread_private.h"
#include "bthread.h"
#include <stdio.h>

#define NULL 0

bthread_t tid0, tid1, tid2, tid3;
long counterA, counterB, counterC;

void thread_supervisor(void *arg) {
    bthread_sleep(2000);
    bthread_cancel(tid0);
    bthread_cancel(tid1);
    bthread_cancel(tid3);
}

//Test per preemption con priority scheduling
void thread_routineA(void *arg) {
    set_priority(2);
    for(;;) {
        bthread_testcancel();
        counterA++;
    }

    bthread_printf("Thread con id %d sta terminando\n", (long) arg);
}

void thread_routineB(void *arg) {
    set_priority(1000);
    for(;;) {
        bthread_testcancel();
        counterB++;
    }

    bthread_printf("Thread con id %d sta terminando\n", (long) arg);
}

void thread_routineC(void *arg) {
    set_priority(80);

    for(;;) {
        bthread_testcancel();
        counterC++;
    }

    bthread_printf("Thread con id %d sta terminando\n", (long) arg);
}


void thread_test() {
    /* Creazione di piu' thread */
    bthread_create(&tid0, NULL, thread_routineA, (void*) 0);
    bthread_create(&tid1, NULL, thread_routineB, (void*) 1);
    bthread_create(&tid3, NULL, thread_routineC, (void*) 3);
    bthread_create(&tid2, NULL, thread_supervisor, (void*) 2);

    bthread_join(tid0, NULL);
    bthread_join(tid1, NULL);
    bthread_join(tid2, NULL);
    bthread_join(tid3, NULL);

    /* Stampa per controllo funzionamento priority scheduling */
    printf("counterA=%d, counterB=%d, counterC=%d\n", counterA, counterB, counterC);
}
