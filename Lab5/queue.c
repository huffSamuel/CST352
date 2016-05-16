#include "queue.h"

my_queue_t * my_q_init()
{
    my_queue_t * queue = (my_queue_t *)malloc(sizeof(my_queue_t));
    assert(queue != NULL);
    queue->count = 0;
    queue->first = NULL;
    queue->last = NULL;
    queue->closed = 0;
    pthread_mutex_init(&(queue->lock), NULL);

    return queue;
}

int my_q_enqueue(my_queue_t * queue, char * value)
{
    if(queue == NULL) return 1;
    node_t * item = (node_t *)malloc(sizeof(node_t));
    item->value = value;
    item->next = NULL;

    if(item == NULL) return 2;

    pthread_mutex_lock(&(queue->lock));

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
    pthread_mutex_unlock(&(queue->lock));
    return 0;
}

char * my_q_dequeue(my_queue_t * queue)
{
    char * value; 
    node_t * item;

    pthread_mutex_lock(&(queue->lock));
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

    pthread_mutex_unlock(&(queue->lock));
    return value;
}

int my_q_cleanup(my_queue_t * queue)
{
    node_t * item;
    while(queue->count > 0)
    {
        item = queue->first;
        queue->first = item->next;
        queue->count--;
        free(item);
    }

    free(queue);
    return 0;
}

int my_q_close(my_queue_t * queue)
{
    pthread_mutex_lock(&(queue->lock));
    queue->closed = 1;
    pthread_mutex_unlock(&(queue->lock));
    return 0;
}

int my_q_is_open(my_queue_t * queue)
{
    int value = 0;
    pthread_mutex_lock(&(queue->lock)); 
    if(queue->closed == 1 && queue->count == 0)
        value = 1;

    pthread_mutex_unlock(&(queue->lock));
    return value;
}