/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      memory.c
* Date Created:  2016-04-26
* Modified:      Samuel Huff 5/1/16
**************************************************************/
/*************************************************************
 * Lab/Assignment: CST 352 Lab 4: Threading Part 1
 *
 * Overview
 *    This program tests a thread-safe list implementation.
 *
 *  Input:
 *    This program takes a single argument: the number of items to add
 *    to the list.
 *
 *  Output:
 *    The program will print error messages if any problems are found with 
 *    the list implementation.
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "list.h"

#define DEFAULT_COUNT 1000
#define DEFAULT_THREADS 1

//******************************************************************
// struct that holds parameters sent to a thread
typedef struct
{
    list_t *list;       // The list to be operated on
    int count;          // The number of items the thread is to add to list
    int thread;         // Thread number
} params_t;

/********************************************************************** 
* Purpose: Validate a queue after running the test program
*
* Precondition: 
*   list points to a well formed list
*   count is the expected number of elements in the list
*   num_threads is the number of threads used to create the list
*
* Postcondition: 
*   returns zero if any errors were found, returns one otherwise
*
************************************************************************/
int validate_queue(list_t *list, int count, int num_threads)
{
    int ii;
    int *seq = (int *)malloc(num_threads * sizeof(int));
    int value;
    int thread;
    int n_errors = 0;

    for (ii=0; ii<num_threads; ii++)
    {
        seq[ii] = 0;
    }


    if (list->count != count)
    {
        fprintf(stderr, "Invalid list count: %d\n", list->count);
        n_errors++;
    }

    while (list->count>0 && list->first != NULL)
    {
        value = list_pop(list);
        thread = ((value&0x0F000000) >> 24) & 0x000F;
        value &= 0x00FFFFFF;

        if (thread >= num_threads)
        {
            fprintf(stderr, "Invalid thread number in %d\n", ii);
            n_errors++;
        }
        else
        {
            if (value != seq[thread] || value >= count/num_threads)
            {
                fprintf(stderr, "Invalid seq %d for thread %d\n", 
                        seq[thread], thread);
                n_errors++;
            }
            seq[thread] = value + 1;
        }
    } 

    if (n_errors == 0) return 1;
    return 0;
}

/********************************************************************** 
* Purpose: validates a list after the test program was run
*
* Precondition: 
*   list points to a well formed list
*   count is the expected number of elements in the list
*
* Postcondition: 
*   returns one if no errors were found, zero if errors were found
*
************************************************************************/
int validate_list(list_t *list, int count)
{
    int ii;
    int n_errors = 0;
    list_item_t *item;

    if (list->count != count)
    {
        fprintf(stderr, "Invalid list count: %d\n", list->count);
        n_errors++;
    }

    item = list->first;
    for (ii=0; ii<list->count-1; ii++)
    {
        if (item == NULL || item->next == NULL || 
            item->value > item->next->value)
        {
            fprintf(stderr, "Invalid data at position %d\n", ii);
            n_errors++;
        }
        item = item->next;
    } 

    if (n_errors == 0) return 1;
    return 0;
}

/********************************************************************** 
* Purpose: returns a number suitable for using as a random number seed
*
* Precondition: 
*   None
*
* Postcondition: 
*   None
*
************************************************************************/
static unsigned int get_seed()
{
    struct timespec time;
    unsigned int seed;

    clock_gettime(CLOCK_MONOTONIC, &time);
    seed = (unsigned int)time.tv_nsec;

    return seed;
}
/********************************************************************** 
* Purpose: Function that can be used by a thread to perform list operations
*
* Precondition: 
*   p points to a params_t struct initalized with parameters for the thread
*
* Postcondition: 
*   list has been updated
*
************************************************************************/
void *perform_operations(void *p)
{
    unsigned int seed = get_seed();
    params_t *params = (params_t *)p;
    int ii;
    int value;

    printf("Count: %d\n", params->count);

    for (ii=0; ii<params->count; ii++)
    {
        value = rand_r(&seed) % (params->count + 1); // (2*params->count + 17);
        list_sorted_insert(params->list, value);
    }

    return NULL;
}
/********************************************************************** 
* Purpose: function that can be run as a thread to perform queue operations
*
* Precondition: 
*   p points to a param_t struct initialized with the parameters for the thread
*
* Postcondition: 
*   List has been updated
*
************************************************************************/
void *perform_queue_operations(void *p)
{
    int ii;
    int value;
    params_t *params = (params_t *)p;

    printf("Count: %d\n", params->count);

    for (ii=0; ii<params->count; ii++)
    {
        // Place the thread number in the upper byte of the word
        value = (params->thread << 24) + ii;
        list_push_end(params->list, value);
    }

    return NULL;
}
/********************************************************************** 
* Purpose: Tests a multi-threaded list implementation
*
* Precondition: 
*   None
*
* Postcondition: 
*   None
*
************************************************************************/
int main(int argc, char **argv)
{
    int count;
    int i;                  // Loop counter
    int threads;            // Number of threads
    int err;                // Error return value 
    list_t *list;
    params_t params;

    if (argc > 2)
    {
        count = atoi(argv[1]);
        threads = atoi(argv[2]);
    }
    else
    {
        count = DEFAULT_COUNT;
        threads = DEFAULT_THREADS;
    }

    pthread_t tid[threads]; // Thread ID array

    printf("Count: %d\n", count);
    printf("Threads: %d\n", threads);

    // Initialize the list
    list = list_init();

    // Initialize the parameter struct
    
    for(i = 0; i < threads; ++i)
    {
        // Setup parameters
        params.list = list;
        params.count = count;
        params.thread = i;

        // Create the thread
        err = pthread_create( &tid[i], NULL, perform_queue_operations, &params );
        if(err != 0)
        {
            printf("Unable to create thread %d", err);
        }
    }

    for(i = 0; i < threads; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    //perform_queue_operations(&params);

    if (validate_queue(list, count, 1))
        printf("Order OK\n");
    else
        printf("Test failed\n");

    return 0;
}
