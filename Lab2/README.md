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

-------
