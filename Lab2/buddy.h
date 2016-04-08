typedef struct
{
  short size;
  short offset;
} block_t;

typedef struct
{
  short m_size;			// Number of bytes for addresses in this bucket
  char m_numUsed;       // Number of this order used. Max is 64 16 byte fields
  char * m_offset;		// True offset is calculated by m_offset << 4
} bucket_t;

void Add(bucket_t * bucket, int size, int addr);
void allocate(int size);
int roundUp(int size);
int computeOrder(int size);
void my_mem_init(bucket_t * free_list, bucket_t * busy_list);
void my_mem_cleanup(bucket_t * free_list, bucket_t * busy_list);
int findBuddy(int addr, short size);
void my_print_mem(bucket_t * free_list, bucket_t * busy_list);