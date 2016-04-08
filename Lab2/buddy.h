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

void allocate(int size);
int roundUp(int size);
int computeOrder(int addr, int size);
void my_mem_init(bucket_t * free);
int findBuddy(int addr, short size);