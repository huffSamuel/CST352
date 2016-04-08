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
int * mem_base;
int allocated;
int main(int argc, char * argv[])
{
  allocated = 0;
  mem_base = malloc(2048);

  bucket_t free_list[8];
  bucket_t busy_list[8];
  my_mem_init(free_list, busy_list);
  my_print_mem(free_list, busy_list);
  my_mem_cleanup(free_list, busy_list); 

  printf("Table size: %d\n", allocated);
}

void my_mem_init(bucket_t * free_list, bucket_t * busy_list)
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
    busy_list[i].m_offset = malloc((1 << (8-i)) * sizeof(char));

    allocated += (int)2*((1 << (8-i)) * sizeof(char));
    free_list[i].m_offset = malloc((1 << (8-i)) * sizeof(char));
    printf("Allocated %d bytes\n", (int)((1 << (8-i)) * sizeof(char)));
  }

  //Add(free_list, 2048, 0);
  Add(free_list, 1024, 0);
  Add(free_list, 1024, 1024);
}

void my_mem_cleanup(bucket_t * free_list, bucket_t * busy_list)
{
  int i;
  for(i = 0; i < 8; i++)
  {
    free(busy_list[i].m_offset);
    free(free_list[i].m_offset);
  }

  free(mem_base);
}

void my_print_mem(bucket_t * free_list, bucket_t * busy_list)
{
  int i;
  int j;
  for(i=0; i < 8; i++)
  {
    printf("free %#06x = ", free_list[i].m_size);
    if(free_list[i].m_numUsed == 0)
      printf("0x0000");
    else
    {
      for(j = 0; j < free_list[i].m_numUsed; j++)
      {
        printf("0x%04x ", free_list[i].m_offset[j] << 4);
      }
    }
    printf("\n");
  }
}

int findBuddy(int addr, short size)
{
  return (addr ^ 1 << (computeOrder(size) + 4));
}

void my_free(int addr)
{
  int offset = addr - *mem_base;
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
  bucket[order].m_offset[used] = addr >> 4;
  bucket[order].m_numUsed = used + 1;
}