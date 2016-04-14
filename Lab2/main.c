#include "buddy.h"

int main(int argc, char * argv[])
{
    my_mem_init();
    intptr_t a = (intptr_t)my_malloc(1024);
    printf("%d", (int)a);
    my_print_mem();
    my_mem_cleanup();
    return 0;
}