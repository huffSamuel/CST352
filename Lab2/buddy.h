
#include "bucket_t.h"

// Base functions for my_mem_stuff
void my_mem_init();
void my_mem_cleanup();
void * my_malloc(int size);
void my_print_mem();
void my_free(void * ptr);

// Helper functions
int roundUp(int size);
int computeOrder(int size);
int findBuddy(int addr, short size);
int printInList(int addr, int order, int listID);
void printFree();
int printBusy(int addr, int order);

void coalesce(int addr, int size);
intptr_t allocate(int size);
