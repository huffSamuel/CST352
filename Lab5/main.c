/*************************************************************
 * Author:        Philip Howard
 * Filename:      prod_cons.h
 * Date Created:  5/4/2016
 * Modifications: 
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

#include "prod_cons.h"

int main(int argc, char **argv)
{
    int n_consumers;
    int ii;
    queue_t queue;

    if (argc < 3)
    {
        fprintf(stderr, "Must specify the number of threads "
                        "and at least on file\n");
        return 1;
    }

    n_consumers = atoi(argv[1]);
    if (n_consumers < 1)
    {
        fprintf(stderr, "Must specify the number of threads "
                        "and at least on file\n");
        return 1;
    }

    queue = Q_Init();

    for (ii=0; ii<n_consumers; ii++)
    {
        pthread_create(consumer);
    }

    for (ii=2; ii<argc; ii++)
    {
        pthread_create(producer, argv[ii]);
    }

    for (ii=2; ii<argc; ii++)
    {
        pthread_join(producer[ii]);
    }

    Q_Close(queue);

    for (ii=0; ii<n_consumers; ii++)
    {
        pthread_join(consumer[ii]);
    }

    Q_Destroy(queue);

    return 0;
}
