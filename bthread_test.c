//
// Created by Elia Oggian on 15.06.17.
//

#include "bthread_test.h"
#include "bthread_private.h"
#include "bthread.h"
#include <stdio.h>

#define NULL 0

bthread_t t0, t1, t2, t3;
long counterA = 0, counterB = 0, counterC = 0;

void thread_supervisor(void *arg) {
    set_priority(1);
    bthread_sleep(2000);
    bthread_cancel(t0);
    bthread_cancel(t1);
    bthread_cancel(t3);
}

void thread_routineA(void *arg) {
    set_priority(2);
    while(1) {
        bthread_testcancel();
        counterA++;
    }

}

void thread_routineB(void *arg) {
    set_priority(1000);
    while(1) {
        bthread_testcancel();
        counterB++;
    }

}

void thread_routineC(void *arg) {
    set_priority(40);
    while(1) {
        bthread_testcancel();
        counterC++;
    }

}


void thread_test() {
    printf("Sequential scheduling\n");


    bthread_get_scheduler()->scheduling_routine = sequential_scheduling;

    bthread_create(&t0, NULL, thread_routineA, (void*) 0);
    bthread_create(&t1, NULL, thread_routineB, (void*) 1);
    bthread_create(&t3, NULL, thread_routineC, (void*) 3);
    bthread_create(&t2, NULL, thread_supervisor, (void*) 2);

    bthread_join(t1, NULL);
    bthread_join(t3, NULL);
    bthread_join(t0, NULL);
    bthread_join(t2, NULL);

    printf("counterA=%d, counterB=%d, counterC=%d\n\n\n", counterA, counterB, counterC);


    printf("Random scheduling\n");

    bthread_get_scheduler()->scheduling_routine = random_scheduling;

    bthread_create(&t0, NULL, thread_routineA, (void*) 0);
    bthread_create(&t1, NULL, thread_routineB, (void*) 1);
    bthread_create(&t3, NULL, thread_routineC, (void*) 3);
    bthread_create(&t2, NULL, thread_supervisor, (void*) 2);

    bthread_join(t1, NULL);
    bthread_join(t3, NULL);
    bthread_join(t0, NULL);
    bthread_join(t2, NULL);

    printf("counterA=%d, counterB=%d, counterC=%d\n\n\n", counterA, counterB, counterC);


    printf("Priority scheduling\n");
    printf("Priorities => A = 2; B = 1000; C = 40\n");

    bthread_get_scheduler()->scheduling_routine = priority_scheduling;

    bthread_create(&t0, NULL, thread_routineA, (void*) 0);
    bthread_create(&t1, NULL, thread_routineB, (void*) 1);
    bthread_create(&t3, NULL, thread_routineC, (void*) 3);
    bthread_create(&t2, NULL, thread_supervisor, (void*) 2);

    bthread_join(t1, NULL);
    bthread_join(t3, NULL);
    bthread_join(t0, NULL);
    bthread_join(t2, NULL);

    printf("counterA=%d, counterB=%d, counterC=%d\n\n\n", counterA, counterB, counterC);

}
