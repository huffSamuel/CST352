#pragma once
#include <sys/types.h>

#define FS_BLOCK_SIZE 1024
#define FS_BLOCK_SUPERBLOCK 1
#define BUSY 0
#define FREE 1


typedef struct
{
    u_int32_t free;             // 0 = inode in use
    u_int32_t inode_number;
    u_int32_t mode;             // not currently used
    u_int32_t link_count;       // not currently used
    u_int32_t size;             // size in bytes
    u_int32_t disk_map[13];
} inode_t;

typedef struct
{
    u_int32_t block[256];
} indirect_block_t; 

typedef struct
{
    u_int32_t free_list[50];        // list of free blocks
    u_int32_t free_inode_list[10];  // list of free inodes
    int32_t reserved;
    int32_t num_free_blocks;
    int32_t num_free_inodes;
    u_int32_t pad[256-63];          // padding to make this 1024 bytes
} super_block_t;

// Close the file system
// Return zero on success
int FS_Close();

// Open the file system
// filename: is the file where the virtual disk is located
// Return zero on success
int FS_Open(const char *filename);

// Create the file system. 
// This is equivalent to formatting the drive
// filename: is the file where the virtual disk will live
// fs_size: is the desired number of blocks in the fs
//
// Return zero on success
int FS_Create(const char *filename, u_int32_t fs_size);

// Read a block from the disk
// buff: location where data is stored
// block: the absolute block number to be read
//
// Returns number of bytes read (FS_BLOCK_SIZE).
// Returns zero if you attempt to read past the end of the file
// Returns a negative number on error
int FS_Read(void *buff, u_int32_t block);

// Write a block to the disk
// buff: location where data is stored
// block: the absolute block number to be read
//
// Returns number of bytes written (FS_BLOCK_SIZE).
// Returns a negative number on error
int FS_Write(void *buff, u_int32_t block);

// Read an inode from the disk
// inode: location to store the inode
// index: inode number to read
//
// Returns zero on success
int FS_Read_Inode(inode_t *inode, u_int32_t index);

// Write an inode to the disk
// inode: pointer to the inode to be written
//
// Returns zero on success
int FS_Write_Inode(inode_t *inode);

// Read a block from a file
// inode: inode of the file to be read
// buff: address where the data is to be stored
// index: block number of the file to be read.
//        NOTE: index is block of file, not absolute disk block
//
// Returns zero if you attempt to read past end-of-file
// Returns the number of bytes read on success (FS_BLOCK_SIZE)
// Returns a negative value on error
int FS_Read_File_Block(inode_t *inode, void *buff, u_int32_t block);

// Write a block to a file
// inode: inode of the file to be written
// buff: address of the data to be written
// index: block number of the file to be written
//        NOTE: index is block of file, not absolute disk block
//
// Returns the number of bytes written on success (FS_BLOCK_SIZE)
// Returns a negative value on error
int FS_Write_File_Block(inode_t *inode, void *buff, u_int32_t block);

// Allocate an unused inode
// This is the equivalent of creating a file
// inode: address where newly allocated inode is to be stored
//
// Return zero on success
int FS_Alloc_Inode(inode_t *inode);

// Free a no-longer used inode
// This is the equivalent of deleting a file
// inode: pointer to inode to free
//
// Return zero on success
int FS_Free_Inode(inode_t *inode);

// Allocate an unused disk block
//
// Return absolute disk block that was allocated
// Return: negative value on failure
int FS_Alloc_Block();

// Free a no-longer used block
// block: absolute disk block to be freed
//
// Return zero on success
int FS_Free_Block(u_int32_t block);
