/*************************************************************
* Author:        Samuel Huff
* Email:         Samuel.Huff@oit.edu
* Filename:      queue.h
* Date Created:  5/12/2016
* Modified:      
**************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include <string.h>

//******************************************
// Definition of Thread Control Block
typedef struct thread_s
{
    int status;
    unsigned long sp;                   // Stack pointer for thread
    unsigned long fp;                   // Frame pointer for thread
    long *stack;                        // ptr to block of memory used for stack
    void *(*start_func)(void *arg);     // function to run in thread
    void *arg;                          // arg to be passed to start_func
} thread_t;

typedef struct node_s
{
    thread_t * value;
    struct node_s * next;
} node_t;

typedef struct
{
    int count;
    struct node_s * first;
    struct node_s * last;
    sem_t *mutex;
    int closed;
} my_queue_t;
/********************************************************************** 
 * Purpose: Initialize a queue for consumer code use. 
 *
 * Precondition: 
 *     None.
 *
 * Postcondition: 
 *      Returns a queue with initialized values.
 ************************************************************************/
my_queue_t * my_q_init();
/********************************************************************** 
 * Purpose: Adds a node to the end of the queue.
 *
 * Precondition: 
 *     queue is an initialized queue.
 *     value is the string value for the node.
 *
 * Postcondition: 
 *      The node is added to the end of the queue. Returns a 0 on success
 *      and a non-zero value on failure.
 ************************************************************************/
int my_q_enqueue(my_queue_t * queue, thread_t * value);
/********************************************************************** 
 * Purpose: Removes an element from the front of the queue.
 *
 * Precondition: 
 *     queue is an initialized queue.
 *
 * Postcondition: 
 *      returns the string of the first node. If the queue is empty returns
 *      null.
 ************************************************************************/
thread_t * my_q_dequeue(my_queue_t * queue);
/********************************************************************** 
 * Purpose: Dealocates memory associated with the queue.
 *
 * Precondition: 
 *     queue is an initialized queue.
 *
 * Postcondition: 
 *      Elements of the queue are freed.
 ************************************************************************/
int my_q_cleanup(my_queue_t * queue);
/********************************************************************** 
 * Purpose: Sets a queue as closed.
 *
 * Precondition: 
 *     Queue is an initialized queue.
 *
 * Postcondition: 
 *      Returns 0 if the queue is successfully marked as closed.
 ************************************************************************/
int my_q_close(my_queue_t * queue);
/********************************************************************** 
 * Purpose: Checks if a queue is currently open.
 *
 * Precondition: 
 *     Queue is a valid queue.
 *
 * Postcondition: 
 *      Returns 1 if the queue is closed and empty. Returns 0 otherwise.
 ************************************************************************/
int my_q_is_open(my_queue_t * queue);
/********************************************************************** 
 * Purpose: Initializes a semaphore.
 *
 * Precondition: 
 *     Value is the initial value of the semaphore.
 *
 * Postcondition: 
 *      The returned semaphore pointer is initialized.
 ************************************************************************/
sem_t * make_semaphore(int value);
/********************************************************************** 
 * Purpose: Waits for a semaphore.
 *
 * Precondition: 
 *     Sem is an initialized semaphore.
 *
 * Postcondition: 
 *      Semaphore has been acquired.
 ************************************************************************/
int semaphore_wait(sem_t *sem);
/********************************************************************** 
 * Purpose: Posts that a thread is finished with semaphore-locked data.
 *
 * Precondition: 
 *     Sem is an initialized semaphore
 *
 * Postcondition: 
 *      Semaphore is released by the thread.
 ************************************************************************/
int semaphore_post(sem_t *sem);
