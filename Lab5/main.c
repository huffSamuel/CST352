/*************************************************************
 * Author:        Philip Howard
 * Filename:      prod_cons.h
 * Date Created:  5/4/2016
 * Modifications: 5/15/2016 Samuel Huf
 **************************************************************
 *
 * Lab/Assignment: Lab 5 - Producer Consumer
 * 
 * Overview:
 *    This program is a producer-consumer application. 
 *    The producers read text files and send the text, one line at a time,
 *    to the consumers. The consumers print the test along with a 
 *    thread-id to identify which thread printed each line.
 *
 * Input:
 *    Command line arguments are used to specify the number of consumers and
 *    the filenames of the files to be read by producers. One producer will 
 *    be started for each filename.
 *
 * Output:
 *    The lines of text from the collected input files.
 *    Note: If there are multiple consumers, there is no guarantee as to the 
 *    order of the output lines.
 *
 * NOTE: this is PSEUDO-CODE, and it will have to be turned into real code
 * to complete this lab.
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "prod_cons.h"

int main(int argc, char **argv)
{
    int n_consumers;
    int ii;
    int err;
    queue_t queue;

    if (argc < 3)
    {
        fprintf(stderr, "Must specify the number of threads "
                        "and at least one file\n");
        return 1;
    }

    n_consumers = atoi(argv[1]);
    if (n_consumers < 1)
    {
        fprintf(stderr, "Must specify the number of threads "
                        "and at least one file\n");
        return 1;
    }

    queue = Q_Init();
    pthread_t consumerIDs[n_consumers];
    pthread_t producerIDs[argc - 2];
    p_params_t producerParams[argc - 2];
    c_params_t consumerParams[n_consumers];

    for (ii=0; ii<n_consumers; ii++)
    {
        consumerParams[ii].queue = queue;
        err = pthread_create( &(consumerIDs[ii]),
            NULL,
            consumer,
            &consumerParams[ii]);
        if(err != 0) printf("Unable to create thread\n");
    }


    for (ii=2; ii<argc; ii++)
    {
        producerParams[ii - 2].queue = queue;
        producerParams[ii - 2].filename = argv[ii];

        err = pthread_create(&producerIDs[ii - 2],
            NULL,
            producer,
            &producerParams[ii - 2]);

        if(err != 0) printf("Unable to create thread\n");
    }

    for (ii=2; ii<argc; ii++)
    {
        pthread_join(producerIDs[ii-2], NULL);
    }

    Q_Close(queue);

    for (ii=0; ii<n_consumers; ii++)
    {
        pthread_join(consumerIDs[ii], NULL);
    }

    Q_Destroy(queue);

    return 0;
}
