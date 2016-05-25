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
    exit(0);

    return NULL;
}

int main()
{
    mythread_init();

    mythread_create(thread_test, (void *)5);
    mythread_create(thread_test, (void *)10);

    while (1)
    {
        mythread_yield();
    }

    printf("main is going away\n");

    return 0;
}

