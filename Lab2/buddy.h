typedef struct
{
  short size;
  short offset;
} block_t;

typedef struct
{
  int m_size;
  int m_numUsed;
  int * m_offset;
} bucket_t;

void Add(bucket_t * bucket, int size, int addr);
void allocate(int size);
int roundUp(int size);
int computeOrder(int size);
void my_mem_init(bucket_t * free_list, bucket_t * busy_list, void * mem_base);
int findBuddy(int addr, short size);

