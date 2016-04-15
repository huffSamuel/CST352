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
* Overview: This lab simulates memory allocation using the buddy system.
*   The program has a limit of 2048 bytes of memory to allocate and 
*   responds as malloc() and free() via the methods my_malloc() and my_free().
*   These functions allocate and free memory inside the 2048 block.
*
* Input:
*   None required.
*
* Output:
*   Memory is allocated and freed as the user requires.
*
*   Error -1: Unknown fatal error
*   Error 1: Unable to free requested memory.
*   Error 2: Unable to allocate managed/list/bucket memory.
*   Error 3: Unable to print to stdout.
****************************************************************/
void * mem_base;            // Our memory to manage
bucket_t * free_list;       // List of free memory locations
bucket_t * busy_list;       // List of busy memory locations
/********************************************************************** 
* Purpose: Initialized memory for my_malloc
*
* Precondition: 
*   free_list is a global bucket_t * pointer
*   busy_list is a global bucket_t * pointer
*   mem_base is a global int * pointer. This is the memory we will be managing.
*
* Postcondition: 
*   mem_base is allocated. 
*   free_list is allocated and contains one block of 2048
*   busy_list is allocated and contains no memory blocks
*
************************************************************************/
void my_mem_init()
{
    int i = 0;      // Loop counter     

    // Allocate our memory chunk
    mem_base = malloc(MEM_SIZE);
    if(mem_base == 0)
    {
        if(fprintf(stderr, "Unable to allocate base memory\n") <= 0)
            exit(-1);
        exit(2);
    }

    // Allocate the free and busy lists
    free_list = malloc(8 * sizeof(bucket_t));
    busy_list = malloc(8 * sizeof(bucket_t));
    if(free_list == 0 || busy_list == 0)
    {
        if(fprintf(stderr, "Unable to allocate list memory\n") <= 0)
            exit(-1);
        exit(2);
    }

    // Allocate and initialize contents of the tables
    for(i = 0; i < 8; i++)
    {
        busy_list[i].m_size = 1 << (i + 4);
        free_list[i].m_size = 1 << (i + 4);
        busy_list[i].m_count = 0;
        free_list[i].m_count = 0;
        busy_list[i].m_offset = malloc((1 << (8-i)) * sizeof(char));
        free_list[i].m_offset = malloc((1 << (8-i)) * sizeof(char));
        if(free_list[i] == 0 || busy_list[i] == 0)
        {
            if(fprintf(stderr, "Unable to allocate bucket memory\n") <= 0)
                exit(-1);
            exit(2);
        }
    }

    // Add the initial 2048 block to the free_list
    Add(free_list, MEM_SIZE, 0);
}

/********************************************************************** 
* Purpose: Deallocates all memory allocated for memory management.
*
* Precondition: 
*   free_list is a global bucket_t * pointer and is allocated.
*   busy_list is a global bucket_t * pointer and is allocated.
*   mem_base is a global int * pointer and is allocated.
*
* Postcondition: 
*   busy_list has all its members freed.
*   free_list has all its members freed.
*   free_list, busy_list, and mem_base are freed.
*
************************************************************************/
void my_mem_cleanup()
{
    int i = 0;      // Loop counter

    // Free list members
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
* Purpose: Validates memory to make sure only 2048 bytes have been allocated.  
*
* Precondition: 
*   None
*
* Postcondition: 
*   None if successful.
*
*   Prints an error on failure.
*
************************************************************************/
void my_validate()
{
    int i = 0;          // Loop counter
    int total = 0;      // Byte counter

    // Travers free and busy lists
    for(i = 0; i < computeOrder(MEM_SIZE) + 1; ++i)
    {
        total += free_list[i].m_count * (16 << i);
        total += busy_list[i].m_count * (16 << i);
    }

    // If an error is found
    if(total != MEM_SIZE)
    { 
        if(printf("Memory error occurred\n") <= 0)
        {
            if(fprintf("Unable to print to stdout") <= 0)
                exit(-1);
            exit(3);
        }
    }
}

/********************************************************************** 
* Purpose: Prints the current state of memory
*
* Precondition: 
*   free_list is a global bucket_t * pointer and is allocated.
*   busy_list is a global bucket_t * pointer and is allocated.
*
* Postcondition: 
*   All elements in free_list are printed.
*   TODO: Make blocks print by ascending offset
************************************************************************/
void my_print_mem()
{
    int i = 0;      // Loop counter
    int addr = 0;   // Address counter

    // Print the contents of free memory
    printf("\n");
    printFree();

    // Print blocks by ascending address
    if(printf("Address | Size | Status\n") <= 0)
    {
        if(fprintf("Unable to print to stdout") <= 0)
            exit(-1);
        exit(3);
    }
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
*   <addr> is the address in memory to check
*   <order> is the order of the address
*   <listID> indicates which list to check
*
* Postcondition: 
*   If the address exists in that list the information is printed
*   and the function returns 1.
*
*   Returns 0 if address not found in list[order]
************************************************************************/
int printInList(int addr, int order, int listID)
{
    int j = 0;      // Loop counter

    // Pick list based on listID
    bucket_t * list = (listID == 1 ? free_list : busy_list);
    
    // Traverse the buckets
    if(list[order].m_count > 0)
    {
        // Traverse the order
        for(j = 0; j < list[order].m_count; ++j)
        {
            if(list[order].m_offset[j] == addr)
            {
                if(printf("0x%04x   %5d  ", addr << 4, list[order].m_size) <= 0)
                {
                    if(fprintf("Unable to print to stdout") <= 0)
                        exit(-1);
                    exit(3);
                }
                if(listID)
                {
                    if(printf(" free\n") <= 0)
                    {
                        if(fprintf("Unable to print to stdout") <= 0)
                            exit(-1);
                        exit(3);
                    }
                }
                else
                {
                    if(printf(" busy\n") <= 0)
                    {
                        f(fprintf("Unable to print to stdout") <= 0)
                            exit(-1);
                        exit(3);
                    }
                }
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
*   Prints all offsets currently in free_list
************************************************************************/
void printFree()
{
    int i = 0;      // Order loop counter
    int j = 0;      // Offset loop counter

    // Traverse free_list
    for(i=0; i < 8; i++)
    {
        if(printf("free %#06x = ", free_list[i].m_size) <= 0)
        {
            if(fprintf("Unable to print to stdout") <= 0)
                exit(-1);
            exit(3);
        }
        if(free_list[i].m_count == 0)
        {
            if(printf("EMPTY") <= 0)
            {
                if(fprintf("Unable to print to stdout") <= 0)
                    exit(-1);
                exit(3);
            }
        }
        else
        {
            for(j = 0; j < free_list[i].m_count; j++)
            {
                if(printf("0x%04x ", free_list[i].m_offset[j] << 4) <= 0)
                {
                    if(fprintf("Unable to print to stdout") <= 0)
                        exit(-1);
                    exit(3);
                }
            }
        }
        if(printf("\n") <= 0)
        {
            if(fprintf("Unable to print to stdout") <= 0)
                exit(-1);
            exit(3);
        }
    }     
}

/********************************************************************** 
* Purpose: Finds the buddy of a block
*
* Precondition: 
*   addr is the offset address of the block.
*   size is the size of the block associated with addr
*
* Postcondition: 
*   Returns the address of <addr, size> block's buddy block
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
*   mem_base is allocated.
*   <addr> is the location the user wishes to free.
*
* Postcondition: 
*   The block in mem_base at address <addr> is freed.
*   Error: Unrecoverable. Kill the process
************************************************************************/
void my_free(void * ptr)
{
    int i = 0;          // Loop counter for buckets
    int j = 0;          // Loop counter for orders in a bucket
    int k = 0;          // Found flag
    int count = 0;      // Number of elements in a bucket
    int offset = 0;     // Offset we are searching for

    if(ptr == NULL)
        return;

    offset = (ptr - mem_base) >> 4;

    // Traverse bucket_t of busy_list
    for (i = 7; i >= 0; --i)
    {
         count = busy_list[i].m_count;
         // Traverse the order
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
        if(fprintf(stderr, "My_Seg_Fault: 0x%08x \n", (uint)(intptr_t)ptr) <= 0)
            exit(-1);
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
    int buddyAddr = (findBuddy(addr, size) >> 4);   // Buddy address
    int order = computeOrder(size);                 // Order we are searching
    int flag = -1;                                  // Found flag
    int i = 0;                                      // Loop counter

    // Check if buddy is joinable
    for(i = 0; i < free_list[order].m_count - 1 && flag == -1; ++i)
    {
        if(free_list[order].m_offset[i] == buddyAddr)
            flag = Join(order, buddyAddr, addr);
    }

    // If joined successfully recurse for further joins
    if(flag != -1)
    {
        coalesce(flag, size << 1);
    }
}

/********************************************************************** 
* Purpose: Allocates memory inside of mem_base.
*
* Precondition: 
*   mem_base is allocated.
*   <size> is the block size that the user wishes to allocate.
*
* Postcondition: 
*   If there is space in memory a block of <size> is allocated. The
*   actual address in memory is returned to the user.
*
************************************************************************/
void * my_malloc(int size)
{
    intptr_t addr = 0;      // Returned address
    int offset = 0;         // Offset into free list to allocate
    
    // Calculate base 2 value or return invalid
    if(size < 16)
        size = 16;
    else if(size > MEM_SIZE)
        return (void *)NULL;
    else 
        size = roundUp(size);

    int order = computeOrder(size);

    // Find a free space from available or splitting
    offset = free_list[order].m_count;
    if(offset > 0)
        addr = free_list[order].m_offset[offset-1] << 4;
    else
        addr = split(size << 1);

    // Return NULL if insufficient space
    if(addr == -1)
        return (void *)NULL;

    // Perform the move
    offset = free_list[order].m_count;
    Move(busy_list, free_list, order, offset);

    return (void *)(addr + mem_base);
}

/********************************************************************** 
* Purpose: Rounds a size up to its nearest power of 2
*
* Precondition: 
*   <size> is the block size to be rounded up.
*
* Postcondition: 
*   If size is less than 16 it is rounded to 16 bytes (minimum block size)
*   If size is greater than 2048 we return an error (maximum block size)
*   Otherwise, size is converted to its next largest power of two.
*
*   Credit: StackOverflow for the algorithm
************************************************************************/
int roundUp(int size)
{
    // Round to minimum size
    if(size < MIN_BLOCK)
        return MIN_BLOCK;
    // Bit twiddle hack for nearest power of 2
    else if(size <= MEM_SIZE)    
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
    // Overflow error
    else
    {
        return -1; // Error code from function
    }
}

/********************************************************************** 
* Purpose: Computes the order of a block where Order(0) is 16B
*   Order(1) is 2 * 16B, Order(N) is N * 2 * 16B     
*
* Precondition: 
*   <size> is the size of the block to compute order on.
*
* Postcondition: 
*   Returns the proper order for block of <size>
*
************************************************************************/
int computeOrder(int size)
{
    int currentSize = MIN_BLOCK;        // Current block size
    int order = 0;                      // Current order
    while(currentSize < size)
    {
        currentSize <<= 1;
        order++;
    }

    return order;
}

