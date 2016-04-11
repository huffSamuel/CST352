typedef struct
{
  short size;
  short offset;
} block_t;

typedef struct
{
  short m_size;			// Number of bytes for addresses in this bucket
  char m_count;       // Number of this order used. Max is 64 16 byte fields
  char * m_offset;		// True offset is calculated by m_offset << 4
} bucket_t;

// Base functions for my_mem_stuff
void my_mem_init();
void my_mem_cleanup();
void * my_malloc(int size);
void my_print_mem();
void my_free(void * ptr);

// Helper functions
void Add(bucket_t * bucket, int size, int addr);
void Remove(bucket_t * bucket, int size);
intptr_t split(int size);
intptr_t allocate(int size);
int roundUp(int size);
int computeOrder(int size);
int findBuddy(int addr, short size);