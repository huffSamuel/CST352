#include <stdio.h>
#include <stdlib.h>

#include "sched.h"

// function we are going to run as a thread
void *thread_test(void *arg)
{
    long count = (long)arg;

    while (--count)
    {
        printf("Counting down: %d\n", count);
        mythread_yield();
    }

    printf("Thread is exiting!\n");

    return NULL;
}

int main()
{
    unsigned long thread_1;
    unsigned long thread_2;

    mythread_init();

    thread_1 = mythread_create(thread_test, (void *)5);

    thread_2 = mythread_create(thread_test, (void *)10);

    mythread_join(thread_1, NULL);
    mythread_join(thread_2, NULL);

    printf("main is going away\n");

    mythread_cleanup();

    return 0;
}

