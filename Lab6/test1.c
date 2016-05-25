#include <stdio.h>
#include <stdlib.h>

#include "sched.h"

// function we are going to run as a thread
void *thread_test(void *arg)
{
    printf("Thread is running!\n");
    exit(0);

    return NULL;
}

int main()
{
    unsigned long thread_1;

    mythread_init();

    thread_1 = mythread_create(thread_test, 0);

    mythread_yield();

    printf("main is going away\n");

    return 0;
}

