// James Jessen
// 10918967   
// 
// CptS 360

// ------------------------------------
// Functions for getting data from and
// putting data onto an ext2 filesystem
// ------------------------------------

#ifndef __GET_PUT_H_
#define __GET_PUT_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ext2fs/ext2_fs.h>

u8*    get_block(int device, int block);
SUPER* get_super(int device); 
GD*    get_gd   (int device);
u8*    get_bmap (int device);
u8*    get_imap (int device);
INODE* get_inode(int device, int inode_number);

void put_block(int device, int block, u8* buf);
void put_super(int device, SUPER* buf); 
void put_gd   (int device, GD* buf);
void put_bmap (int device, u8* buf);
void put_imap (int device, u8* buf);
//void put_inode(int device, INODE* buf, int inode_number);

int get_magic       (int device);
int get_block_size  (int device);
int get_blocks_count(int device);
int get_inodes_count(int device);


#endif
