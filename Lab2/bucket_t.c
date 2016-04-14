#include "bucket_t.h"

#define DEBUG_MODE 1
#define MEM_SIZE 2048
#define MIN_BLOCK 16
extern bucket_t * free_list;
extern bucket_t * busy_list;

extern int computeOrder(int size);

/********************************************************************** 
* Purpose: Adds a block to a memory list.
*
* Precondition: 
*   <bucket> is the memory list to add the element to.
*   <size> is the size of the block.
*   <addr> is the actual offset of the block.
*
* Postcondition: 
*   <addr> is adapted to short offset
*   <bucket> now contains the entry for block(size, addr)
*
************************************************************************/
void Add(bucket_t * bucket, int size, int addr)
{
    int order = computeOrder(size);
    int used = bucket[order].m_count;
    bucket[order].m_offset[used] = addr >> 4;
    bucket[order].m_count = used + 1;
}

/********************************************************************** 
* Purpose: Splits a block of size <size> into 2 blocks of <size>/2
*
* Precondition: 
*   <size> is the size of a block we wish to split.
*
* Postcondition: 
*   Splits a block of size <size> if available. Returns the higher address 
*   of the split blocks.
*
*   Error: Returns -1
*
*   RECURSIVE
************************************************************************/
intptr_t split(int size)
{
    int check = 0;
    int order = computeOrder(size);

    if(size > MEM_SIZE) return -1;

    int count = free_list[order].m_count;
    int offset;

    if(count == 0) // Split one of the size up
    {
        check = split(size << 1);
        if(check == -1) 
            return -1;
        count = free_list[order].m_count;
    }

    offset = (free_list[order].m_offset[count - 1]) << 4;

    free_list[order].m_count = count - 1;
    Add(free_list, size>>1, offset);
    Add(free_list, size>>1, offset + (size >> 1));
    return offset + (size >> 1);
}

// Working here
intptr_t Join(int order, int addr, int addrB)
{
    int i;
    int a = -1;
    int b = -1;
    int srcCount = free_list[order].m_count;
    // Place in higher order
    int destCount = free_list[order+1].m_count;
    

    // Find lower order offsets
    for(i = 0; i < srcCount && (b == -1 || a == -1); ++i)
    {
        if(free_list[order].m_offset[i] == addrB)
            b = i;
        else if(free_list[order].m_offset[i] == addr)
            a = i;
    }

    // Return fail message
    if(b == -1 || a == -1) return -1;

    free_list[order+1].m_offset[destCount] = addr > addrB ? addrB : addr;
    free_list[order+1].m_count = destCount + 1;

    // Remove from lower order
    srcCount -= 2;
    free_list[order].m_count = srcCount;
    if(a < srcCount && b < srcCount)
    {
        free_list[order].m_offset[a] = free_list[order].m_offset[srcCount+1];
        free_list[order].m_offset[b] = free_list[order].m_offset[srcCount+2];
    }
    else if(a < srcCount)
    {
        b = (b == srcCount + 1) ? srcCount + 2 : srcCount + 1;
        free_list[order].m_offset[a] = free_list[order].m_offset[b];
    }
    else if(b < srcCount)
    {
        a = (a == srcCount + 1) ? srcCount + 2 : srcCount + 1;
        free_list[order].m_offset[b] = free_list[order].m_offset[a];
    }

    return free_list[order+1].m_offset[destCount];
}

void Move(bucket_t * dest, bucket_t * source, int order, int offset)
{
    int destCount = dest[order].m_count;
    int srcCount = source[order].m_count;

    // Insert at end of destination
    dest[order].m_offset[destCount] = source[order].m_offset[offset-1];
    dest[order].m_count = destCount + 1;
    source[order].m_count = srcCount - 1;

    // Move end of source list into offset's spot if necessary
    if(offset < srcCount)
    {
        source[order].m_offset[offset-1] = source[order].m_offset[srcCount - 1];
    }
}