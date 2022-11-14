#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <ext2fs/ext2fs.h>

void printSuperBlock(ext2_filsys fs);
unsigned int blockSize(unsigned int);
unsigned int clusterSize(unsigned int, unsigned int);

#endif