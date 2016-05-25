#pragma once

//****************************************
// User level cooperative thread library

// Must be called prior to any other mythread routines
void mythread_init();

// Can be called to clean up any memory used by thread data structures.
// This function assumes all threads have finished.
void mythread_cleanup();

// Roughly equivalent to pthread_create
// func is the function to be run in the thread.
// arg is the argument to pass to the func
// returns the thread_id of the created thread
unsigned long mythread_create( void *(*func)(void *arg), void *arg);

// Threads call this to yield the CPU
void mythread_yield();

// Roughly the equivalent of pthread_exit
// result is the return value for the thread
void mythread_exit(void *result);

// Roughly the equivalent of pthread_join
// thread_id is the thread to wait for. 
//    if thread_id does not identify a valid thread, the behavior is undefined
// result can be used to get the result of the thread.
//    if result==NULL, the return value of the thread will be discarded
void mythread_join(unsigned long thread_id, void **result);

// Roughly the equivalent of pthread_detach
// thread_id is the ID of the thread to detach
void mythread_detach(unsigned long thread_id);
