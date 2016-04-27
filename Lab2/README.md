#CST352 Lab 2 : Memory Allocation

The purpose of this lab is to familarize yourself with the workings of a memory allocator.

## Requirements

Implement a buddy system allocator that meets the following requirements:

1. Memory size is 2048 bytes.
2. Minimum block size is 16 bytes.
3. Allocating and freeing must follow the format of malloc() and free().
4. Implement *my_validate()* that checks for and informs the user of memory corruption.
5. Implement *my_print_mem()* that prints the current contents of your 2048 block.
6. Metadata for the 2048 block must exist in an external table.

## Grade and Notes

Lab score: 95/100
- It is best if file scope variables are NOT shared between files. When you have to do that, it suggests you didn't factor your code appropriately.

- You had hard-coded magic numbers throughout your code. You should define MEMORY_SIZE and MIN_BLOCK_SIZE and compute everything relative to them.

Paranoia score: 29/30

- in my_free, you computed the offset but never did any sanity checks. They were somewhat implied by not finding the computed offset in the free list, but it would be better to bounds check before searching.
