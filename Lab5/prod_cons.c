/*************************************************************
 * Author:        Philip Howard
 * Filename:      prod_cons.h
 * Date Created:  5/4/2016
 * Modifications: 
 **************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#include "prod_cons.h"
#include "queue.h"

/********************************************************************** 
 * Purpose: This function initializes a queue. 
 * It returns an opaque pointer to the queue. The queue must be destroyed when
 * no longer needed by calling Q_Destroy()
 *
 * Precondition: 
 *     None
 *
 * Postcondition: 
 *      Queue has been created.
 *      Returns NULL on failure
 *
 ************************************************************************/
queue_t Q_Init()
{   
    return (queue_t)my_q_init();
}

/********************************************************************** 
 * Purpose: This function cleans up any memory occupied by the queue. 
 * It should only be called when the queue is no longer needed, and is no
 * longer being accessed by any other threads.
 *
 * Precondition: 
 *     The queue is a valid queue that has been closed and emptied.
 *     No other threads are currently accessing the queue, and none will in
 *     the future.
 *
 * Postcondition: 
 *      Queue has been destroyed; all memory has been reclaimed.
 *      Returns zero on success and non-zero on failure
 *
 ************************************************************************/
int Q_Destroy(queue_t queue)
{
    return my_q_cleanup((my_queue_t*) queue);
}

/********************************************************************** 
 * Purpose: This markes the queue as closed. Dequeue operations are allowed
 * after a queue is marked as closed, but no further enqueue operations should
 * be performed.
 *
 * Precondition: 
 *     Queue is a valid queue.
 *
 * Postcondition: 
 *      Queue has been marked as closed.
 *      Returns zero on success, non-zero on failure
 ************************************************************************/
int Q_Close(queue_t q)
{
    return my_q_close((my_queue_t*)q);
}

/********************************************************************** 
 * Purpose: Places a new element into the queue.
 *
 * Precondition: 
 *     Queue is a valid queue that has not been marked as closed.
 *
 * Postcondition: 
 *      Queue contains one additional element
 *      Returns zero on success, non-zero on failure
 ************************************************************************/
int Q_Enqueue(queue_t queue, char *buffer)
{
    return my_q_enqueue((my_queue_t *)queue, buffer);
}

/********************************************************************** 
 * Purpose: Removes an element from the queue
 *
 * Precondition: 
 *     Queue is a valid queue
 *
 * Postcondition: 
 *      If the queue was not empty, it contains one less element
 *      Returns a pointer to the string stored in the queue.
 *      Returns NULL if the queue is empty.
 *      NOTE: This behavior will be changed as part of this lab
 ************************************************************************/
char *Q_Dequeue(queue_t queue)
{
    return my_q_dequeue((my_queue_t *)queue);
}

/********************************************************************** 
 * Purpose: Indicates whether the queue is open
 *
 * Precondition: 
 *     Queue is a valid queue
 *
 * Postcondition: 
 *      Returns zero if either the queue has not been marked as close OR
 *                             the queue is not empty
 *      Returns non-zero of BOTH: the queue has been marked as closed AND
 *                                the queue is empty
 ************************************************************************/
int Q_Is_Open(queue_t queue)
{
    return my_q_is_open((my_queue_t*)queue);
}

/********************************************************************** 
 * Purpose: Acts as a producer that reads from a file and places the data
 * from the file into the queue one line at a time.
 *
 * Precondition: 
 *     Queue is a valid queue
 *     filename is a valid filename for which the process has read access
 *
 * Postcondition: 
 *      The file has been read and the lines are places into the queue
 ************************************************************************/
void *producer(void * p)
{
    p_params_t * params = (p_params_t *)p;
    queue_t queue = params->queue;
    char * filename = params->filename;
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    char * buffer = NULL;
    char * pos = NULL;
    int i = 0;

    buffer = (char *)malloc(256*sizeof(char));
    while(fgets(buffer, 256, fp))
    {
        for(i = 0; i < strlen(buffer); ++i)
        {
            if(!isspace(buffer[i])) break;
        }
        if(i < strlen(buffer))
        {
            if ((pos=strchr(buffer, '\n')) != NULL)
                *pos = '\0';
            Q_Enqueue(queue, buffer);
            buffer = (char *)malloc(256*sizeof(char));
        }
    }
    free(buffer);
    fclose(fp);
    return NULL;
}

/********************************************************************** 
 * Purpose: Acts as a consumer that reads from a queue and writes the lines 
 * pulled from the queue to stdout.
 *
 *
 * Precondition: 
 *     Queue is a valid queue
 *
 * Postcondition: 
 *      The file has been closed and emptied.
 ************************************************************************/
void *consumer(void * p)
{
    c_params_t * params = (c_params_t*)p;
    queue_t queue = params->queue;

    pthread_t tid;
    tid = pthread_self();
    char * value;
    while(Q_Is_Open(queue) != 1)
    {
        value = Q_Dequeue(queue);
        if(value != NULL)
        {
            printf("%d %s\n", (int)tid, value);
            free(value);
        }
    }

    return NULL;
}

