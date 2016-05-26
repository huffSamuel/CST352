#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "sched.h"
#include "queue.h"
// use a 1M stack
#define STACK_SIZE  (1024*1024)
#define ACTIVE 1
#define EXITING 0


// Pointer to currently running thread
thread_t *Current_Thread;
thread_t * Previous_Thread;
my_queue_t * Ready_Queue;

/**********************************************************************
 * Purpose:  This function can be used for adding debug statements to your code.
 * Debug statements can then be turned on/off with the Do_Debug flag.
 *
 * Precondition:
 *     None
 *
 * Postcondition:
 *      text printed to stderr if Do_Debug is true
 ************************************************************************/
static int Do_Debug = 0;
static void debug_print(const char *fmt, ...)
{
    if (!Do_Debug) return;

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);
    va_end(args);
}
/**********************************************************************
 * Purpose:  This function can be used for adding debug statements to your code.
 * Debug statements can then be turned on/off with the Do_Debug flag.
 *
 * Precondition:
 *     None
 *
 * Postcondition:
 *      text printed to stderr if Do_Debug is true
 ************************************************************************/
void mythread_init()
{
    // Create TCB for whoever is initing me
    if(Ready_Queue == NULL)
        Ready_Queue = my_q_init();

    Current_Thread = (thread_t *)malloc(sizeof(thread_t));
    assert(Current_Thread != NULL);

    memset(Current_Thread, 0, sizeof(thread_t));

    Current_Thread->sp = 0;
    Current_Thread->fp = 0;
    Current_Thread->stack = NULL;
    Current_Thread->start_func = NULL;
    Current_Thread->arg = NULL;
    Current_Thread->status = ACTIVE;
}
// //*************************************
// Function that gets called on thread startup. It calls the main thread
// function and then stops the thread.
/**********************************************************************
 * Purpose: This function gets called on thread startup. It calles the
 * user supplied function and then exits the thread.
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *      Current_Thread points to a newly created TCB
 *
 * Postcondition:
 *      User's thread has completed
 ************************************************************************/
static void start_thread_func()
{
    void *result;
    result = Current_Thread->start_func(Current_Thread->arg);

    mythread_exit(result);
}

/**********************************************************************
 * Purpose: This function will create a new user thread
 *      func is the function to run in the thread
 *      arg is the argument to be passed to the function
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      The user's thread has been created
 *      return: the thread_id of the newly created thread is returned
 ************************************************************************/
unsigned long mythread_create( void *(*func)(void *arg), void *arg)
{
    long *mem;
    thread_t *thread;

    // allocate the thread control block
    thread = (thread_t *)malloc(sizeof(thread_t));
    assert(thread != NULL);

    memset(thread, 0, sizeof(thread_t));

    // Allocate space for the stack
    thread->stack = (long *)malloc(STACK_SIZE*sizeof(long));

    // Remember the user's thread function and argument
    thread->start_func = func;
    thread->arg = arg;

    // Initialize the stack. Pretend we made a call to yield 
    // from the beginning of start_thread_func
    //
    // NOTE: The following code, even though written in C is machine specific
    mem = thread->stack;

    mem[STACK_SIZE-1] = (long)start_thread_func;        // return addr
    mem[STACK_SIZE-2] = (long)&mem[STACK_SIZE-2];       // frame ptr
    thread->fp = (long)&mem[STACK_SIZE-2];              // save FP
    thread->sp = (long)&mem[STACK_SIZE-2];              // save SP
    thread->status = ACTIVE;

    my_q_enqueue(Ready_Queue, thread);
    // a call to yield will now run this thread
    mythread_yield();
    return 0;   // should return thread_id
}

/**********************************************************************
 * Purpose: This function is called by threads in order to yield the CPU
 * NOTE: This code is CPU and compiler option specific
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      none
 ************************************************************************/
void mythread_yield()
{
    
    unsigned long reg;

    debug_print("Thread is yielding\n");
    if(Current_Thread->status != EXITING)
    {
        // save FP, SP and queue current thread in ready queue
        __asm__ volatile("movq %%rbp, %0" : "=m" (reg) : :);
        Current_Thread->fp = reg;
        __asm__ volatile("movq %%rsp, %0" : "=m" (reg) : :);
        Current_Thread->sp = reg;
        my_q_enqueue(Ready_Queue, Current_Thread);
    }
    else Previous_Thread = Current_Thread;
    // Get the next thread
    
    Current_Thread = my_q_dequeue(Ready_Queue);

    debug_print("Thread is running\n");

    // restore the SP, FP
    reg = Current_Thread->sp;
    __asm__ volatile("movq %0, %%rsp" : : "m" (reg) :);

    reg = Current_Thread->fp;
    __asm__ volatile("movq %0, %%rbp" : : "m" (reg) :);

    if(Previous_Thread != NULL && Previous_Thread->status == EXITING)
    {
        free(Previous_Thread->stack);
        free(Previous_Thread);
        Previous_Thread = NULL;
    }

    // return to next thread
    return;
}
/**********************************************************************
 * Purpose: This function will terminate the thread that called it
 *      result: the return value of the thread
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      The user's thread has been terminated.
 *      If the thread is detached, the memory for the thread will be cleaned up
 *      If the thread is not detached, result is available for a caller to
 *          mythread_join.
 ************************************************************************/
void mythread_exit(void *result)
{
    debug_print("Thread is exiting\n");
    // clean up thread data structures
    Current_Thread->status = EXITING;

    mythread_yield();
}
/**********************************************************************
 * Purpose: This function will wait for the specified thread to terminate.
 *      thread_id: the identifier of the thread to wait for
 *      result: location to store the thread's result.
 *              if result==NULL, the thread's result will be ignored
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *      thread_id is the ID of a thread that is not detached and has not already
 *          joined.
 *
 * Postcondition:
 *      The user's thread has finished
 *      The result of the user's thread is in result (if result != NULL)
 ************************************************************************/
void mythread_join(unsigned long thread_id, void **result)
{
}
/**********************************************************************
 * Purpose: This function will mark a thread a detached meaning that the
 *      thread's memory will be cleaned up immediately upon thread_exit()
 *
 *      thread_id: the ID of the thread to be detached
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *      thread_id has not already been detached and has not already finished
 *
 * Postcondition:
 *      The user's thread has been marked as detached
 ************************************************************************/
void mythread_detach(unsigned long thread_id)
{
}
/**********************************************************************
 * Purpose: This function shut down the user thread system and clean-up any
 *      memory used by the threading system
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      Memory has been reclaimed.
 *      the behavior of any calls to mythread_ functions after this call 
 *      is undefined
 ************************************************************************/
void mythread_cleanup()
{
   my_q_cleanup(Ready_Queue);
   free(Current_Thread->stack);
   free(Current_Thread); 
}

