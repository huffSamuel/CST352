/****************************************************************
* Author: Samuel
* Filename: bucket_t.c
* Date Created: April 7th, 2016
* Modifications: 
****************************************************************/
#include "bucket_t.h"

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
    int order = computeOrder(size);             // Order of the added block
    int used = bucket[order].m_count;           // Number blocks used in the bucket
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
    if(size > MEM_SIZE) return -1;

    int order = computeOrder(size);         // Order of the requested block
    int count = free_list[order].m_count;   // Number of blocks of that order
    int offset;                             // Placeholder for offset to store

    // If none available split next size up
    if(count == 0)
    {
        if(split(size << 1) == -1)
            return -1;
        count = free_list[order].m_count;
    }

    offset = (free_list[order].m_offset[count - 1]) << 4;

    // Add new blocks to next order down
    free_list[order].m_count = count - 1;
    Add(free_list, size>>1, offset);
    Add(free_list, size>>1, offset + (size >> 1));

    return offset + (size >> 1);
}

/********************************************************************** 
* Purpose: Joins to equivalent sized blocks and places them in the higher order
*
* Precondition: 
*   <order> is the order of the two blocks.
*   <addr> is the address of one of the buddys.
*   <addrB> is the address of the other buddy.
*
* Postcondition: 
*   If blocks are joinable it joins the blocks and places them in the 
*   free list with a higher order. Returns the address of the new block.
*
*   Unsuccessful join: Return -1
************************************************************************/
intptr_t Join(int order, int addr, int addrB)
{
    int i = 0;      // Offset loop counter
    int a = -1;     // Flag for address A found + a's offset
    int b = -1;     // Flag for address B found + b's offset
    int srcCount = free_list[order].m_count;    // Elements in source offset
    int destCount = free_list[order+1].m_count; // Elements in destination offset

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

    // Place in higher order
    free_list[order+1].m_offset[destCount] = addr > addrB ? addrB : addr;
    free_list[order+1].m_count = destCount + 1;

    // Remove from lower order
    srcCount -= 2;
    free_list[order].m_count = srcCount;
    
    removeOrder(order, a, b, srcCount);

    return free_list[order+1].m_offset[destCount];
}

/********************************************************************** 
* Purpose: Removes two blocks from an order of the free list.
*
* Precondition: 
*   <order> is the order of the two blocks.
*   <addr> is the offset of one of the buddys.
*   <addrB> is the offset of the other buddy.
*   <srcCount> is the number of objects in the source bucket
*
* Postcondition: 
*   Both blocks are removed and list is reordered as necessary.
************************************************************************/
void removeOrder(int order, int a, int b, int srcCount)
{
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
}

/********************************************************************** 
* Purpose: Moves a block from source bucket to destination bucket
*
* Precondition: 
*   <dest> is the destination bucket.
*   <source> is the source bucket.
*   <order> is the order of the block to move.
*   <offset> is the offest into the source bucket to move.
*
* Postcondition: 
*   Both blocks are removed and list is reordered as necessary.
************************************************************************/
void Move(bucket_t * dest, bucket_t * source, int order, int offset)
{
    int destCount = dest[order].m_count;    // Elements in destingation order
    int srcCount = source[order].m_count;   // Elements in source order

    // Insert at end of destination
    dest[order].m_offset[destCount] = source[order].m_offset[offset-1];
    dest[order].m_count = destCount + 1;
    source[order].m_count = srcCount - 1;

    // Move end of source list into offset's spot if necessary
    if(offset < srcCount)
        source[order].m_offset[offset-1] = source[order].m_offset[srcCount - 1];
}