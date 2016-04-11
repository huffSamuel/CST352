/****************************************************************
* Author: Samuel
* Filename: buddy.c
* Date Created: April 7th, 2016
* Modifications: 
*
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
int main(int argc, char * argv[])
{
  allocated = 0;
  intptr_t offset = 0;

  my_mem_init();
  my_print_mem(free_list, busy_list);
  allocate(2048);
  offset = split(1024);
  my_mem_cleanup(free_list, busy_list); 
  

  if(DEBUG_MODE)
    printf("Table size: %d\n", allocated);

}


/********************************************************************** 
* Purpose: Initialized memory for my_malloc
*
* Precondition: 
*      free_list is a global bucket_t * pointer
*      busy_list is a global bucket_t * pointer
*      mem_base is a global int * pointer. This is the memory we will be 
*        managing.
*
* Postcondition: 
*      mem_base is allocated. 
*      free_list is allocated and contains one block of 2048
*      busy_list is allocated and contains no memory blocks
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
}

/********************************************************************** 
* Purpose: Deallocates all memory allocated for memory management.
*
* Precondition: 
*      free_list is a global bucket_t * pointer and is allocated.
*      busy_list is a global bucket_t * pointer and is allocated.
*      mem_base is a global int * pointer and is allocated.
*
* Postcondition: 
*      busy_list has all its members freed.
*      free_list has all its members freed.
*      free_list, busy_list, and mem_base are freed.
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
*      free_list is a global bucket_t * pointer and is allocated.
*      busy_list is a global bucket_t * pointer and is allocated.
*
* Postcondition: 
*      All elements in free_list are printed.
*      TODO: Make blocks print by ascending offset
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
*      addr is the offset address of the block.
*      size is the size of the block associated with addr
*
* Postcondition: 
*      Returns the address of <addr, size> block's buddy block
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
*      mem_base is allocated.
*      <addr> is the location the user wishes to free.
*
* Postcondition: 
*      The block in mem_base at address <addr> is freed.
*
************************************************************************/
void my_free(void * ptr)
{
  int offset = ptr - mem_base;
  // Move the memory thing to the free table
  // Coalesce (recursive)
}

/********************************************************************** 
* Purpose: Allocates memory inside of mem_base.
*
* Precondition: 
*      mem_base is allocated.
*      <size> is the block size that the user wishes to allocate.
*
* Postcondition: 
*      If there is space in memory a block of <size> is allocated. The
*      actual address in memory is returned to the user.
*
************************************************************************/
void * my_malloc(int size)
{
  intptr_t addr;
  if(size < 16)
    size = 16;
  else
    size = roundUp(size);

  // 
  
  return (void *)addr;
}

// TODO: Bad Logic. 
intptr_t allocate(int size)
{
  int order = computeOrder(size);
  int numUsed = free_list[order].m_count;

  //if(size > MEM_SIZE)
  //  return -1;

  //if(numUsed == 0)
  //  split(size);


  // Free slots, allocate here
  //else if(numUsed >= (128 >> order))
  //{
  //  intptr_t addr = (free_list[order].m_offset[numUsed - 1] << 4) + (intptr_t)mem_base; 
  //  free_list[order].m_count = numUsed - 1;
    // Move that to busy_list 
  //  return addr;
  //}
  //else
  //{
  //  intptr_t addr = allocate(size << 1);
  //  return addr;
  //}  
}

intptr_t split(int size)
{
  int order = computeOrder(size);
  int count = free_list[order].m_count;
  int offset;

  if(size > MEM_SIZE) return -1;
  else if(count == 0) // Split one of the size up
  {
    split(size << 1);
    count = free_list[order].m_count;
  }

  offset = (free_list[order].m_offset[count - 1]) << 4;

  free_list[order].m_count = count - 1;
  Add(free_list, size>>1, offset);
  Add(free_list, size>>1, offset + (size >> 1));
  return offset;
}


/********************************************************************** 
* Purpose: Rounds a size up to its nearest power of 2
*
* Precondition: 
*      <size> is the block size to be rounded up.
*
* Postcondition: 
*      If size is less than 16 it is rounded to 16 bytes (minimum block size)
*      If size is greater than 2048 we return an error (maximum block size)
*      Otherwise, size is converted to its next largest power of two.
*
************************************************************************/
int roundUp(int size)
{
  int i = 0;
  if(size < MIN_BLOCK)
    return MIN_BLOCK;
  else if(size <= MEM_SIZE)  // Bit twiddle hack for nearest power of 2
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
*          Order(0) is 16B
*          Order(1) is 2 * 16B
*          Order(N) is N * 2 * 16B   
*
* Precondition: 
*      <size> is the size of the block to compute order on.
*
* Postcondition: 
*      Returns the proper order for block of <size>
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

/********************************************************************** 
* Purpose: Adds a block to a memory list.
*
* Precondition: 
*      <bucket> is the memory list to add the element to.
*      <size> is the size of the block.
*      <addr> is the actual offset of the block.
*
* Postcondition: 
*      <addr> is adapted to short offset
*      <bucket> now contains the entry for block(size, addr)
*
************************************************************************/
void Add(bucket_t * bucket, int size, int addr)
{
  int order = computeOrder(size);
  int used = bucket[order].m_count;
  bucket[order].m_offset[used] = addr >> 4;
  bucket[order].m_count = used + 1;
}