#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "myfs.h"

static int fs_fd = -1;
static int FS_Num_Blocks = 1000;            // in blocks
static int FS_Num_Inodes = 100;             // Max number of files

#define FS_INODES_PER_BLOCK  (FS_BLOCK_SIZE/sizeof(inode_t))

// the Super Block is kept in memory while FS is open
static super_block_t Super_Block;

// Cache of the last block of inodes read
static inode_t FS_Inode_Cache[FS_INODES_PER_BLOCK + 1];

// block number of the block in the Inode_Cache
static int FS_Inode_Cache_Block = -1;

// How many blocks are in the inode area?
#define FS_NUM_INODE_BLOCKS     (FS_Num_Inodes / FS_INODES_PER_BLOCK + 1)

// Index of first Data Block
#define FS_FIRST_DATA_BLOCK     (FS_NUM_INODE_BLOCKS + 2)

// Number of items in the Super Block's free list
// NOTE: This doesn't include slot zero, which is point to next free list
#define FS_FREE_LIST_SIZE       (sizeof(Super_Block.free_list)/  \
                                sizeof(Super_Block.free_list[0]) - 1)

// Number of items in the Super Block's free inode list
#define FS_FREE_INODE_SIZE      (sizeof(Super_Block.free_inode_list)/  \
                                sizeof(Super_Block.free_inode_list[0]))

//*************************************
int FS_Close()
{
    int status;

    status = FS_Write(&Super_Block, 1);
    if (status != FS_BLOCK_SIZE) return status;

    return close(fs_fd);
}

//*************************************
int FS_Open(const char *filename)
{
    int status;

    // indicate no inodes in the cache
    FS_Inode_Cache_Block = -1;

    // file is opened with O_SYNC so all activity gets flushed to disk
    fs_fd = open(filename, O_RDWR | O_SYNC);

    if (fs_fd < 0) return -1;

    // Read the Super Block so we have it in memory
    status = FS_Read(&Super_Block, 1);
    if (status != FS_BLOCK_SIZE) return status;

    return 0;
}

//*************************************
// NOTE: This will destructively format the file system
int FS_Create(const char *filename, u_int32_t fs_size)
{
    // buffer we can use to initialize all blocks on the virtual drive
    char block[FS_BLOCK_SIZE];
    int ii;
    int status;

    // create the file
    // Truncate the file if it already exists
    // NOTE: we do NOT use O_SYNC so that initialation goes fast
    //       we will reopen after init, so the live FS has O_SYNC
    fs_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
    if (fs_fd < 0) return 1;

    FS_Num_Blocks = fs_size;

    // Fill the FS with zeros
    memset(block, 0, FS_BLOCK_SIZE);
    for (ii=0; ii<FS_Num_Blocks; ii++)
    {
        status = write(fs_fd, block, FS_BLOCK_SIZE);
        if (status != FS_BLOCK_SIZE) return -1;
    }

    // Initialize the Super Block
    memset(&Super_Block, 0, sizeof(Super_Block));

    // Free all data blocks
    for (ii=FS_Num_Blocks-1; ii>=FS_FIRST_DATA_BLOCK; ii--)
    {
        FS_Free_Block(ii);
    }

    // Free all inodes
    inode_t inode;
    memset(&inode, 0, sizeof(inode));
    for (ii=0; ii<FS_Num_Inodes; ii++)
    {
        inode.free = 0;             // pretend like inode is in use then
                                    // call FS_Free_Inode to free it
        inode.inode_number = ii;
        FS_Free_Inode(&inode);
    }

    // Close so everything is flushed to disk
    status = FS_Close();
    if (status) return status;

    // Open the live FS
    status = FS_Open(filename);
    if (status) return status;

    return 0;
}

//*************************************
int FS_Read(void *buff, u_int32_t block)
{
    int status;
    
    status = lseek(fs_fd, block*FS_BLOCK_SIZE, SEEK_SET);
    if (status != block*FS_BLOCK_SIZE) return -1;

    status = read(fs_fd, buff, FS_BLOCK_SIZE);
    if (status == FS_BLOCK_SIZE) return status;

    return -1;
}

//*************************************
int FS_Write(void *buff, u_int32_t block)
{
    int status;
    
    status = lseek(fs_fd, block*FS_BLOCK_SIZE, SEEK_SET);
    if (status != block*FS_BLOCK_SIZE) return -1;

    status = write(fs_fd, buff, FS_BLOCK_SIZE);
    if (status == FS_BLOCK_SIZE) return status;

    return -1;
}

//*************************************
int FS_Read_Inode(inode_t *inode, u_int32_t index)
{
    int inode_block;
    int status;
    int offset;

    // what block is the inode stored in?
    // Add 2 to account for boot block and super block
    inode_block = index / FS_INODES_PER_BLOCK + 2;

    // offset of the inode within the block
    offset = index % FS_INODES_PER_BLOCK;

    // if the desired block is NOT already in our cache, read it
    if (inode_block != FS_Inode_Cache_Block)
    {
        status = FS_Read(&FS_Inode_Cache, inode_block);
        if (status != FS_BLOCK_SIZE)
        {
            FS_Inode_Cache_Block = -1;
            return status;
        }
        FS_Inode_Cache_Block = inode_block;
    }

    // copy the requested inode from the cache into user's buffer
    memcpy(inode, &FS_Inode_Cache[offset], sizeof(inode_t));

    return 0;
}
//*************************************
int FS_Write_Inode(inode_t *inode)
{
    int inode_block;
    u_int32_t index;
    int offset;
    int status;

    // get inode number
    index = inode->inode_number;

    // figure out what block the inode lives in
    inode_block = index / FS_INODES_PER_BLOCK + 2;

    // offset of inode within the block
    offset = index % FS_INODES_PER_BLOCK;

    // make sure we have the inode in our cache
    if (inode_block != FS_Inode_Cache_Block)
    {
        status = FS_Read(&FS_Inode_Cache, inode_block);
        if (status != FS_BLOCK_SIZE)
        {
            FS_Inode_Cache_Block = -1;
            return status;
        }
        FS_Inode_Cache_Block = inode_block;
    }

    // copy the inode into the cache
    memcpy(&FS_Inode_Cache[offset], inode, sizeof(inode_t));

    // write back the cache
    status = FS_Write(&FS_Inode_Cache, inode_block);
    if (status != FS_BLOCK_SIZE) return status;

    return 0;
}
//*************************************
int FS_Read_File_Block(inode_t *inode, void *buff, u_int32_t block)
{
    int * mem_status;
    int status = 0;
    indirect_block_t iblock;

    if(block * FS_BLOCK_SIZE > inode->size) return 0;   // Don't read larger than file
    else if(block < 10)
    {
        if(inode->disk_map[block] == 0)
        {
            mem_status = memset(buff, 0, 1024);
            if(mem_status == NULL) return -1;

            return FS_BLOCK_SIZE;
        }
        status = FS_Read(buff, inode->disk_map[block]);
    }
    else if(block < 266)   // Prevent x2 x3 indirection
    {
        block -= 10;
        status = FS_Read(&iblock, inode->disk_map[INDIRECT_BLOCK_MAP]);
        if(status != FS_BLOCK_SIZE) return -1;

        if(iblock.block[block] == 0)
        {
            mem_status = memset(buff, 0, 1024);
            if(mem_status == NULL) return -1;

            return FS_BLOCK_SIZE;
        }
        status = FS_Read(buff, iblock.block[block]);
    }
    else status = -1;

    return status;
}
//*************************************
int FS_Write_File_Block(inode_t *inode, void *buff, u_int32_t block)
{
    int status = 0;
    int temp_block = 0;
    indirect_block_t iblock;

    if(block < 10)
    {
        if(inode->disk_map[block] == 0)
        {
            temp_block = FS_Alloc_Block();
            if(temp_block == -1) return -1;

            inode->disk_map[block] = temp_block;
            inode->size = block * FS_BLOCK_SIZE;

            status = FS_Write_Inode(inode);
            if(status == -1) return status;
        }

        status = FS_Write(buff, inode->disk_map[block]);
    }
    else if(block < 266)    // Prevent x2 x3 indirection
    {
        block -= 10; // Calculate into indirect

        // Allocate indirect block in map if necessary
        if(inode->disk_map[INDIRECT_BLOCK_MAP] == 0)
        {
            temp_block = FS_Alloc_Block();
            if(temp_block == -1) return -1;

            inode->disk_map[INDIRECT_BLOCK_MAP] = temp_block;

            status = FS_Write_Inode(inode);
            if(status == -1) return status;
        }
        status = FS_Read(&iblock, inode->disk_map[INDIRECT_BLOCK_MAP]);
        if(status != FS_BLOCK_SIZE) return -1;

        // Create the block if necessary
        if(iblock.block[block] == 0)
        {
            temp_block = FS_Alloc_Block();
            if(temp_block == -1) return -1;

            iblock.block[block] = temp_block;
            inode->size = (block + 10) * FS_BLOCK_SIZE;

            status = FS_Write(&iblock, inode->disk_map[INDIRECT_BLOCK_MAP]);
            if(status != FS_BLOCK_SIZE) return -1;

            status = FS_Write_Inode(inode);
            if(status == -1) return status;
        }

        status = FS_Write(buff, iblock.block[block]);
    }
    else status = -1;

    return status;
}
//*************************************
// FS_INODES_PER_BLOCK in each of FS_NUM_INODE_BLOCKS
// Super_Block.free_inode_list is cache of inodes
// 
int FS_Alloc_Inode(inode_t *inode)
{
    
    int i = 0;
    int index = Super_Block.free_inode_list[0];
    int block_addr = index / FS_INODES_PER_BLOCK + ILIST_BLOCK;
    int start_block_addr = block_addr;
    int status = 0;
    inode_t inode_block[FS_INODES_PER_BLOCK + 1];
    u_int32_t inode_loc;

    // if (cache empty)
    while(Super_Block.num_free_inodes == 0)
    {
        status = FS_Read(&inode_block, block_addr);
        if(status != FS_BLOCK_SIZE) return -1;
        // Scan for free inode
        for(i = 0; i < FS_INODES_PER_BLOCK; ++i)
        {
            if(inode_block[i].free == FREE 
                && Super_Block.num_free_inodes < FS_FREE_INODE_SIZE)
            {
                Super_Block.free_inode_list[Super_Block.num_free_inodes] = 
                    inode_block[i].inode_number;
                Super_Block.num_free_inodes++;
            }
        }

        block_addr = (block_addr + 1) % FS_NUM_INODE_BLOCKS + ILIST_BLOCK;
        if(block_addr == 0) block_addr = ILIST_BLOCK;

        if(block_addr == start_block_addr)
            return -1;
    }

    inode_loc = Super_Block.free_inode_list[--Super_Block.num_free_inodes];

    status = FS_Read_Inode(inode, inode_loc);
    if(status != 0) return -1;

    inode->free = BUSY;

    status = FS_Write(&Super_Block, FS_BLOCK_SUPERBLOCK);
    if(status != FS_BLOCK_SIZE) return -1;

    status = FS_Write_Inode(inode);

    return status;
}
//*************************************
// if(space)add inode number to SB_inode_cache
// mark inode as free
// write inode 
int FS_Free_Inode(inode_t *inode)
{
    int status;
    int SB_inodes = Super_Block.num_free_inodes;

    if (SB_inodes < FS_FREE_INODE_SIZE)
    {
        Super_Block.free_inode_list[SB_inodes] = inode->inode_number;
        Super_Block.num_free_inodes++;

        status = FS_Write(&Super_Block, FS_BLOCK_SUPERBLOCK);
        if (status != FS_BLOCK_SIZE) return status;
    }

    inode->free = FREE;

    status = FS_Write_Inode(inode);

    return status;
}

//*************************************
int FS_Alloc_Block()
{
    int num_free = Super_Block.num_free_blocks;
    int block = 0;
    int status = 0;

    if(num_free > 1)
    {
        block = Super_Block.free_list[num_free - 1];
        Super_Block.num_free_blocks--;
        status = FS_Write(&Super_Block, FS_BLOCK_SUPERBLOCK);
    }
    else    
    {
        block = Super_Block.free_list[0];
        if(block != 0)
        {
            status = FS_Read(&Super_Block.free_list, block);
            if(status != FS_BLOCK_SIZE) return -1;

            status = FS_Write(&Super_Block, FS_BLOCK_SUPERBLOCK); 
        }
        else status = -1;
    }

    return ((status == FS_BLOCK_SIZE) ? block : -1);
}
//*************************************
int FS_Free_Block(u_int32_t block)
{
    int * mem_status;
    int free_blocks = Super_Block.num_free_blocks;
    int status = 0;

    if(free_blocks < FS_FREE_LIST_SIZE)
    {
        Super_Block.free_list[free_blocks] = block;
        Super_Block.num_free_blocks++;
    }
    else 
    {
        status = FS_Write(&Super_Block, block);
        if(status != FS_BLOCK_SIZE) return -1;

        mem_status = memset(&Super_Block, 0, sizeof(super_block_t));
        if(mem_status == NULL) return -1;

        Super_Block.free_list[0] = block;
        Super_Block.num_free_blocks = 1;
    }
    
    status = FS_Write(&Super_Block, FS_BLOCK_SUPERBLOCK);

    return (status == FS_BLOCK_SIZE ? 0 : -1);
}
