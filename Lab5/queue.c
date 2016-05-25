/*************************************************************
* Author:        Samuel Huff
* Email:         Samuel.Huff@oit.edu
* Filename:      queue.c
* Date Created:  5/12/2016
* Modified:      
**************************************************************/

#include "queue.h"
/********************************************************************** 
 * Purpose: Initialize a queue for consumer code use. 
 *
 * Precondition: 
 *     None.
 *
 * Postcondition: 
 *      Returns a queue with initialized values.
 ************************************************************************/
my_queue_t * my_q_init()
{
    my_queue_t * queue = (my_queue_t *)malloc(sizeof(my_queue_t));
    assert(queue != NULL);
    queue->count = 0;
    queue->first = NULL;
    queue->last = NULL;
    queue->closed = 0;
    
    queue->mutex = make_semaphore(1);
    return queue;
}
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
int my_q_enqueue(my_queue_t * queue, char * value)
{
    if(queue == NULL) return 1;
    node_t * item = (node_t *)malloc(sizeof(node_t));
    item->value = value;
    item->next = NULL;

    if(item == NULL) return 2;
    
    semaphore_wait(queue->mutex);

    if(queue->last == NULL)
    {
        queue->first = item;
        queue->last = item;
    }
    else
    {
        queue->last->next = item;
        queue->last = item;
    }

    queue->count++;
    
    semaphore_post(queue->mutex);

    return 0;
}
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
char * my_q_dequeue(my_queue_t * queue)
{
    char * value;
    node_t * item;

    
    semaphore_wait(queue->mutex);

    if(queue->first != NULL)
    {
        item = queue->first;
        value = item->value;

        queue->first = item->next;
        if(queue->first == NULL) queue->last = NULL;
        queue->count--;

        free(item);
    }
    else
        value = NULL;

    semaphore_post(queue->mutex);
    
    return value;
}
/********************************************************************** 
 * Purpose: Dealocates memory associated with the queue.
 *
 * Precondition: 
 *     queue is an initialized queue.
 *
 * Postcondition: 
 *      Elements of the queue are freed.
 ************************************************************************/
int my_q_cleanup(my_queue_t * queue)
{
    free(queue->mutex);
    free(queue);
    return 0;
}
/********************************************************************** 
 * Purpose: Sets a queue as closed.
 *
 * Precondition: 
 *     Queue is an initialized queue.
 *
 * Postcondition: 
 *      Returns 0 if the queue is successfully marked as closed.
 ************************************************************************/
int my_q_close(my_queue_t * queue)
{
    if(semaphore_wait(queue->mutex) != 0) return 1;
    queue->closed = 1;
    if(semaphore_post(queue->mutex) != 0) return 1;

    return 0;
}
/********************************************************************** 
 * Purpose: Checks if a queue is currently open.
 *
 * Precondition: 
 *     Queue is a valid queue.
 *
 * Postcondition: 
 *      Returns 1 if the queue is closed and empty. Returns 0 otherwise.
 ************************************************************************/
int my_q_is_open(my_queue_t * queue)
{
    int value = 0;
    
    if( semaphore_wait(queue->mutex) != 0) return -1;

    if(queue->closed == 1 && queue->count == 0)
        value = 1;
    
    semaphore_post(queue->mutex);
    return value;
}
/********************************************************************** 
 * Purpose: Initializes a semaphore.
 *
 * Precondition: 
 *     Value is the initial value of the semaphore.
 *
 * Postcondition: 
 *      The returned semaphore pointer is initialized.
 ************************************************************************/
sem_t * make_semaphore(int value)
{
    sem_t * sem = (sem_t*)malloc(sizeof(sem_t));
    int err = sem_init(sem, 0, value);
    if(err != 0) return NULL;
    return sem;
}
/********************************************************************** 
 * Purpose: Waits for a semaphore.
 *
 * Precondition: 
 *     Sem is an initialized semaphore.
 *
 * Postcondition: 
 *      Semaphore has been acquired.
 ************************************************************************/
int semaphore_wait(sem_t *sem)
{
    int err = sem_wait(sem);
    if(err != 0) return 1;
    return 0;
}
/********************************************************************** 
 * Purpose: Posts that a thread is finished with semaphore-locked data.
 *
 * Precondition: 
 *     Sem is an initialized semaphore
 *
 * Postcondition: 
 *      Semaphore is released by the thread.
 ************************************************************************/
int semaphore_post(sem_t *sem)
{
    int err = sem_post(sem);
    if(err != 0) return 1;
    return 0;
}