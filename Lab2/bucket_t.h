#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef struct
{
  short m_size;         // Number of bytes for addresses in this bucket
  uint8_t m_count;       // Number of this order used. Max is 64 16 byte fields
  char * m_offset;      // True offset is calculated by m_offset << 4
} bucket_t;

void Add(bucket_t * bucket, int size, int addr);
void Move(bucket_t * dest, bucket_t * source, int order, int offset);
intptr_t Join(int order, int offsetA, int offsetB);
intptr_t split(int size);
void coalesce(int addr, int size);
void removeOrder(int order, int a, int b, int srcCount);