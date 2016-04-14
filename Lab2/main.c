#include "buddy.h"

int main(int argc, char * argv[])
{
    my_mem_init();
    intptr_t a = (intptr_t)my_malloc(512);
    a = (intptr_t)my_malloc(512);
    my_print_mem();
    printf("Freeing addr %d\n", (int)a);
    my_free((void*)a);
    my_print_mem();
    my_mem_cleanup();
    return 0;
}