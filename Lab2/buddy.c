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
    
    /* Testing 2 chunks
    Add(busy_list, 1024, 0);
    Add(free_list, 1024, 1024);
    */

    /* Testing 4 chucnks
    Add(busy_list, 512, 0);
    Add(free_list, 512, 512);
    Add(busy_list, 512, 1024);
    Add(busy_list, 512, 1536);
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
    int addr;

    // Print the contents of free memory
    printFree();
    // Print blocks by ascending address
    
    printf("Address | Size | Status\n");
    for(addr = 0; addr < 128; )
    {
        for(i = 0; i < 8; ++i)
        { 
            if(printInList(addr, i, 1) || printInList(addr, i, 0))
            {
                addr += (free_list[i].m_size >> 4);
            }
        }
    }
}

/********************************************************************** 
* Purpose: Prints size and free/busy information on <addr> location in memory
*
* Precondition: 
*            <addr> is the address in memory to check
*            <order> is the order of the address
*            <listID> indicates which list to check
*
* Postcondition: 
*            If the address exists in that list the information is printed
*            and the function returns 1.
*
*            Returns 0 if address not found in list[order]
************************************************************************/
int printInList(int addr, int order, int listID)
{
    int j;
    bucket_t * list = (listID == 1 ? free_list : busy_list);
    
    if(list[order].m_count > 0)
    {
        // Traverse the order
        for(j = 0; j < list[order].m_count; ++j)
        {
            if(list[order].m_offset[j] == addr)
            {
                printf("0x%04x   %5d  ", addr << 4, list[order].m_size);
                if(listID)
                    printf(" free\n");
                else
                    printf(" busy\n");
                return 1;
            }
        }
    }
    return 0;
}

/********************************************************************** 
* Purpose: Prints the contents of free memory
*
* Precondition: 
*            
* Postcondition: 
*            Prints all offsets currently in free_list
************************************************************************/
void printFree()
{
    int i;
    int j;
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
        Move(free_list, busy_list, i, j + 1);
        coalesce(offset, 16 << i);
    }
    else
    {
        if(fprintf(stderr, "Failed to free memory") <= 0)
            exit(1);
    }
}

/********************************************************************** 
* Purpose: Coalesces a block(<addr>, <size>) with it's buddy
*
* Precondition: 
*   block(<addr>, <size>) is freed
*
* Postcondition: 
*   If the buddy block is free the blocks will be joined and placed
*   in the free list for the next highest order
************************************************************************/
void coalesce(int addr, int size)
{
    int buddyAddr = (findBuddy(addr, size) >> 4);

    int i;
    int j;
    int count;
    int flag = -1;

    // Traverse each order
    for(i = 7; i >= 0; --i)
    {
        count = free_list[i].m_count;
        // Traverse each address in the order
        for(j = 0; j < count; ++j)
        {
            // If the offset is the proper buddy address
            if(free_list[i].m_offset[j] == buddyAddr)
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
    int offset;
    
    if(size < 16)
        size = 16;
    else
        size = roundUp(size);

    int order = computeOrder(size);


    if(!free_list[order].m_count < (128 >> order))
        addr = split(size << 1);

    if(addr == -1)
    {
        return (void *)-1; // Error in allocating
    }

    offset = free_list[order].m_count;
    Move(busy_list, free_list, order, offset);

    return (void *)(addr + mem_base);
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

