#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

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
    pthread_mutex_t lock;
    int closed;
} my_queue_t;

my_queue_t * my_q_init();

int my_q_enqueue(my_queue_t * queue, char * value);

char * my_q_dequeue(my_queue_t * queue);

int my_q_cleanup(my_queue_t * queue);

int my_q_close(my_queue_t * queue);

int my_q_is_open(my_queue_t * queue);