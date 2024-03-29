./Makefile                                                                                          0000600 0001750 0001750 00000000333 12712546720 010772  0                                                                                                    ustar   sam                             sam                                                                                                                                                                                                                    COPTS = -Wall -g -O0 -c -pthread

OBJS = prod_cons.o \
       queue.o \
       main.o \

all: mytest

clean:
	rm -rf mytest
	rm -rf $(OBJS)

.c.o:
	gcc $(COPTS) $? -o $@

mytest: $(OBJS)
	gcc $(OBJS) -o mytest -pthread
                                                                                                                                                                                                                                                                                                     ./prod_cons.c                                                                                       0000600 0001750 0001750 00000014075 12716271206 011472  0                                                                                                    ustar   sam                             sam                                                                                                                                                                                                                    /*************************************************************
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
    if(fp == NULL) error("failed to open file");
    char * buffer = NULL;
    char * pos = NULL;
    int i = 0;

    buffer = (char *)malloc(256*sizeof(char));
    if(buffer == NULL) error("memory allocation failed");

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
            if(Q_Enqueue(queue, buffer) != 0) error("Enqueue failed");
            buffer = (char *)malloc(256*sizeof(char));
            if(buffer == NULL) error("memory allocation failed");
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
            printf("%u %s\n", (unsigned)tid, value);
            free(value);
        }
    }

    return NULL;
}

void error(char * msg)
{
    if(fprintf(stderr, "Error: %s\n", msg) != strlen(msg))
        exit(1);
    exit(2);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                   ./prod_cons.h                                                                                       0000600 0001750 0001750 00000011601 12717204111 011457  0                                                                                                    ustar   sam                             sam                                                                                                                                                                                                                    /*************************************************************
 * Author:        Philip Howard
 * Filename:      prod_cons.h
 * Date Created:  5/4/2016
 * Modifications: 
 **************************************************************/

typedef void * queue_t;

typedef struct 
{
    queue_t queue;
    char * filename;
} p_params_t;

typedef struct 
{
    queue_t queue;
} c_params_t;

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
queue_t Q_Init();

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
int Q_Destroy(queue_t queue);

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
int Q_Close(queue_t queue);

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
int Q_Enqueue(queue_t queue, char *buffer);

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
char *Q_Dequeue(queue_t queue);

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
int Q_Is_Open(queue_t queue);

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
void *producer(void * p);

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
void *consumer(queue_t queue);

/********************************************************************** 
 * Purpose: Prints an error message to stderr and verifies correct output.
 *
 * Precondition: 
 *     Msg is the error message.
 *
 * Postcondition: 
 *      Msg is printed to stderr. If fprintf fails exits the program with 
 *      error code 1. It exits with error code 2 otherwise.
 ************************************************************************/
void error(char * msg);                                                                                                                               ./queue.c                                                                                           0000664 0001750 0001750 00000012640 12717204227 010636  0                                                                                                    ustar   sam                             sam                                                                                                                                                                                                                    /*************************************************************
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
}                                                                                                ./queue.h                                                                                           0000664 0001750 0001750 00000007642 12717204226 010650  0                                                                                                    ustar   sam                             sam                                                                                                                                                                                                                    /*************************************************************
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

typedef struct node_s
{
    char * value;
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
int my_q_enqueue(my_queue_t * queue, char * value);
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
char * my_q_dequeue(my_queue_t * queue);
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              