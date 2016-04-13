/****************************************************************
* Author: Samuel
* Filename: buddy.c
* Date Created: April 7th, 2016
* Modifications: 
*
****************************************************************/
#include "buddy.h"
#define DEBUG_MODE 1
#define MEM_SIZE 2048
#define MIN_BLOCK 16

/****************************************************************
*
* Lab/Assignment: Lab 2 - Memory Allocator
*
* Overview:
*
* Input:
*
* Output:
*
****************************************************************/
void * mem_base;
bucket_t * free_list;
bucket_t * busy_list;
int allocated;
/*int main(int argc, char * argv[])
{
    intptr_t offset = 0;
    allocated = 0;
    my_mem_init();
    my_print_mem(free_list, busy_list);
    my_mem_cleanup(free_list, busy_list); 

    if(DEBUG_MODE)
        printf("%d bytes allocated\n", allocated);
}*/


/********************************************************************** 
* Purpose: Initialized memory for my_malloc
*
* Precondition: 
*            free_list is a global bucket_t * pointer
*            busy_list is a global bucket_t * pointer
*            mem_base is a global int * pointer. This is the memory we will be 
*                managing.
*
* Postcondition: 
*            mem_base is allocated. 
*            free_list is allocated and contains one block of 2048
*            busy_list is allocated and contains no memory blocks
*
************************************************************************/
void my_mem_init()
{
    // Setup for initialization
    int i;

    // Allocate our memory chunk
    mem_base = malloc(MEM_SIZE);

    // Allocate the free and busy lists
    free_list = malloc(8 * sizeof(bucket_t));
    busy_list = malloc(8 * sizeof(bucket_t));

    // Allocate and initialize contents of the tables
    for(i = 0; i < 8; i++)
    {
        busy_list[i].m_size = 1 << (i + 4);
        free_list[i].m_size = 1 << (i + 4);
        busy_list[i].m_count = 0;
        free_list[i].m_count = 0;
        busy_list[i].m_offset = malloc((1 << (8-i)) * sizeof(char));

        allocated += (int)2*((1 << (8-i)) * sizeof(char));

        free_list[i].m_offset = malloc((1 << (8-i)) * sizeof(char));
    }

    // Add the initial 2048 block to the free_list
    Add(free_list, MEM_SIZE, 0);
    /* FOR TESTING
    Add(free_list, 1024, 0);
    Add(free_list, 1024, 1024);
    */
}

/********************************************************************** 
* Purpose: Deallocates all memory allocated for memory management.
*
* Precondition: 
*            free_list is a global bucket_t * pointer and is allocated.
*            busy_list is a global bucket_t * pointer and is allocated.
*            mem_base is a global int * pointer and is allocated.
*
* Postcondition: 
*            busy_list has all its members freed.
*            free_list has all its members freed.
*            free_list, busy_list, and mem_base are freed.
*
************************************************************************/
void my_mem_cleanup()
{
    // Free list members
    int i;
    for(i = 0; i < 8; i++)
    {
        free(busy_list[i].m_offset);
        free(free_list[i].m_offset);
    }

    // Free lists
    free(busy_list);
    free(free_list);

    // Free memory chunk
    free(mem_base);
}

/********************************************************************** 
* Purpose: Prints the current state of memory
*
* Precondition: 
*            free_list is a global bucket_t * pointer and is allocated.
*            busy_list is a global bucket_t * pointer and is allocated.
*
* Postcondition: 
*            All elements in free_list are printed.
*            TODO: Make blocks print by ascending offset
************************************************************************/
void my_print_mem()
{
    int i;
    int j;

    // Print the contents of free memory
    for(i=0; i < 8; i++)
    {
        printf("free %#06x = ", free_list[i].m_size);
        if(free_list[i].m_count == 0)
            printf("0x0000");
        else
        {
            for(j = 0; j < free_list[i].m_count; j++)
            {
                printf("0x%04x ", free_list[i].m_offset[j] << 4);
            }
        }
        printf("\n");
    }
    // Print blocks by ascending address
    short offset = 0;
    for(i = 0; i < 8; i++) // For each order
    {
        
    }
}

/********************************************************************** 
* Purpose: Finds the buddy of a block
*
* Precondition: 
*            addr is the offset address of the block.
*            size is the size of the block associated with addr
*
* Postcondition: 
*            Returns the address of <addr, size> block's buddy block
*
************************************************************************/
int findBuddy(int addr, short size)
{
    return ((addr << 4) ^ 1 << (computeOrder(size) + 4));
}

/********************************************************************** 
* Purpose: Frees memory from mem_base at offset <addr>
*
* Precondition: 
*            mem_base is allocated.
*            <addr> is the location the user wishes to free.
*
* Postcondition: 
*       The block in mem_base at address <addr> is freed.
*       Error: Unrecoverable. Kill the process
************************************************************************/
void my_free(void * ptr)
{
    int i;
    int j;
    int k;
    int count;
    int offset = (ptr - mem_base) >> 4;
    if(ptr == NULL)
        return;

    for (i = 7; i >= 0; --i)
    {
         count = busy_list[i].m_count;
         for(j = 0; j < count; ++j)
         {
            if(busy_list[i].m_offset[j] == offset)
            {
                k = -1;
                break;
            }
         } 
         if(k == -1) break;
    }
    if(k == -1)
    {
        Move(free_list, busy_list, i, j);
        coalesce(offset, 16 << i);
    }
    else
    {
        if(fprintf(stderr, "Failed to free memory") <= 0)
            exit(1);
    }
}

void coalesce(int addr, int size)
{
    int buddyAddr = (findBuddy(addr << 4, size) >> 4);

    int i;
    int j;
    int count;
    int flag = -1;

    for(i = 7; i >= 0; --i)
    {
        count = free_list[i].m_count;
        for(j = 0; j < count; ++j)
        {
            if(free_list[i].m_offset[j] = buddyAddr)
            {
                flag = Join(i, j, addr);
                break;
            }
        }
        if(flag != -1)
        {
            coalesce(flag, size << 1);
            break;
        }
    }
}

/********************************************************************** 
* Purpose: Allocates memory inside of mem_base.
*
* Precondition: 
*            mem_base is allocated.
*            <size> is the block size that the user wishes to allocate.
*
* Postcondition: 
*            If there is space in memory a block of <size> is allocated. The
*            actual address in memory is returned to the user.
*
************************************************************************/
void * my_malloc(int size)
{
    intptr_t addr;
    if(size < 16)
        size = 16;
    else
        size = roundUp(size);

    // Start allocating memory here
    
    return (void *)addr;
}

// TODO: Bad Logic. 
intptr_t allocate(int size)
{
    int order = computeOrder(size);
    int count = free_list[order].m_count;
}


/********************************************************************** 
* Purpose: Rounds a size up to its nearest power of 2
*
* Precondition: 
*            <size> is the block size to be rounded up.
*
* Postcondition: 
*            If size is less than 16 it is rounded to 16 bytes (minimum block size)
*            If size is greater than 2048 we return an error (maximum block size)
*            Otherwise, size is converted to its next largest power of two.
*
************************************************************************/
int roundUp(int size)
{
    int i = 0;
    if(size < MIN_BLOCK)
        return MIN_BLOCK;
    else if(size <= MEM_SIZE)    // Bit twiddle hack for nearest power of 2
    {
        size--;
        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;
        size++;
        return size;
    }
    else
    {
        return -1; // Error code from function
    }
}

/********************************************************************** 
* Purpose: Computes the order of a block where
*                    Order(0) is 16B
*                    Order(1) is 2 * 16B
*                    Order(N) is N * 2 * 16B     
*
* Precondition: 
*            <size> is the size of the block to compute order on.
*
* Postcondition: 
*            Returns the proper order for block of <size>
*
************************************************************************/
int computeOrder(int size)
{
    int currentSize = MIN_BLOCK;
    int order = 0;
    while(currentSize < size)
    {
        currentSize <<= 1;
        order++;
    }

    return order;
}

