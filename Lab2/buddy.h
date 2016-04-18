#include <stdlib.h>
#include "bucket_t.h"

// Detailed information on functions found in buddy.c

// Initializes 2048 memory chunk and free/busy lists
void my_mem_init();
// Deallocates all memory used
void my_mem_cleanup();
// Emulates functionality of malloc()
void * my_malloc(int size);
// Prints the current status of memory
void my_print_mem();
// Emulates functionality of free()
void my_free(void * ptr);
// Validates memory to check for data corruption
void my_validate();

// Helper functions
// Rounds a size up to the nearest base-2 value
int roundUp(int size);
// Computes the order of a given size
int computeOrder(int size);
// Finds the buddy address of an address and size
int findBuddy(int addr, short size);
// Prints detailed memory values from a given list
int printInList(int addr, int order, int listID);
// Prints the contents of free memory
void printFree();