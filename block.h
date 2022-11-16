#ifndef BLOCK_H
#define BLOCK_H 

#include <ext2fs/ext2fs.h>

int printBlock(ext2_filsys, blk_t);
void printFormattedBuffer(unsigned char*, blk_t);
int dirBlock(ext2_filsys, blk_t);
void printDirectoryBlock(unsigned char*, blk_t);

#endif 
