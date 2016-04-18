#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

// Detailed information on functions found in bucket_t.c

// Structure for storing memory chunk information
typedef struct
{
  short m_size;         // Number of bytes for addresses in this bucket
  uint8_t m_count;      // Number of this order used. Max is 64 16 byte fields
  char * m_offset;      // True offset is calculated by m_offset << 4
} bucket_t;

// Adds a memory value to a bucket
void Add(bucket_t * bucket, int size, int addr);
// Moves a memory value from one bucket to another
void Move(bucket_t * dest, bucket_t * source, int order, int offset);
// Joins two memory values
intptr_t Join(int order, int offsetA, int offsetB);
// Splits a memory value of requested size
intptr_t split(int size);
// Coalesces two memory value into a larger one
void coalesce(int addr, int size);
// Remoes a memory value from an order
void removeOrder(int order, int a, int b, int srcCount);