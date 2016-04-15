#include "buddy.h"

int main(int argc, char * argv[])
{
    my_print_mem();
    intptr_t a[128];
    int i;
    my_mem_init();
    my_print_mem();
    for(i = 0; i < 128; ++i)
    {
        a[i] = (intptr_t)my_malloc(16);
    }
    my_print_mem();
    for(i = 0; i < 128; ++i)
    {
        my_free((void*)a[i]);
    }
    my_print_mem();
    my_validate();
    my_mem_cleanup();
    return 0;
}