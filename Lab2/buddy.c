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

#define MEM_SIZE 2048

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

int main(int argc, char * argv[])
{
  void * mem_base = malloc(2048);
  bucket_t free_list[8];
  bucket_t busy_list[8];
  my_mem_init(free_list, busy_list, mem_base);
  printf("Allocation done");
}

void my_mem_init(bucket_t * free_list, bucket_t * busy_list, void * mem_base)
{
  // Setup for initialization
  int i;

  // Allocate and initialize contents of the tables
  for(i = 0; i < 8; i++)
  {
    busy_list[i].m_size = 1 << (i + 4);
    free_list[i].m_size = 1 << (i + 4);
    busy_list[i].m_numUsed = 0;
    free_list[i].m_numUsed = 0;
    busy_list[i].m_offset = malloc((1 << (8-i)) * sizeof(int));
    free_list[i].m_offset = malloc((1 << (8-i)) * sizeof(int));
  }

  Add(free_list, 2048, 0);

}

int findBuddy(int addr, short size)
{
  return (addr ^ 1 << (computeOrder(size) + 4));
}

void my_free(int addr)
{
  // Move the memory thing to the free table
  // Coalesce (recursive)
}

void my_malloc(int size)
{
  size = roundUp(size);
  allocate(size);
  // Start at max order
  // Check free_list[order] to see if one is free
  // if(numUsed != 2048/size)
  //   Return the address
  // else
  //   my_malloc(size * 2);
}

void allocate(int size)
{
  // check if free_list[computeOrder] has a free slot
  // if(numUsed != (2048/size))
  //   return the address of the block
  //   move the block to busy_table
  // else
  //   allocate(size * 2);
}

int roundUp(int size)
{
  int i = 0;
  if(size < 16)
    return 16;
  else if(size < 2049)  // Bit twiddle hack for nearest power of 2
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

int computeOrder(int size)
{
  int currentSize = 16;
  int order = 0;
  while(currentSize < size)
  {
    currentSize <<= 1;
    order++;
  }

  return order;
}

void Add(bucket_t * bucket, int size, int addr)
{
  int order = computeOrder(size);
  int used = bucket[order].m_numUsed;
  bucket[order].m_offset[used] = addr;
  bucket[order].m_numUsed = used + 1;
}