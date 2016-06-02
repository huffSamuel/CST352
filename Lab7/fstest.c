#include <stdio.h>
#include <string.h>

#include "myfs.h"

static const char *MY_FS_NAME = "my_fs.disk";
static const int  FS_SIZE = 1000;                      // in blocks
//*************************************
int main()
{
    int status;

    status = FS_Create(MY_FS_NAME, FS_SIZE);
    if (status != 0)
    {
        printf("Create_FS failed\n");
        return -1;
    }

    status = FS_Close(MY_FS_NAME);
    if (status != 0)
    {
        printf("Close_FS failed\n");
        return -1;
    }

    status = FS_Open(MY_FS_NAME);
    if (status != 0)
    {
        printf("Open_FS failed\n");
        return -1;
    }

    int block;
    int ii;
    for (ii=0; ii<100; ii++)
    {
        block = FS_Alloc_Block();
        printf("Alloc: %d\n", block);
    }

    for (ii=50; ii<100; ii++)
    {
        FS_Free_Block(ii);
    }

    for (ii=0; ii<100; ii++)
    {
        block = FS_Alloc_Block();
        printf("Alloc: %d\n", block);
    }

    status = FS_Close(MY_FS_NAME);
    if (status != 0)
    {
        printf("Close_FS failed\n");
        return -1;
    }

    return 0;
}
