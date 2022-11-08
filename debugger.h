#ifndef DEBUGGER_H
#define DEBUGGER_H 

#include <ext2fs/ext2fs.h>

#define FEATURE_LEN 10

void printSuperBlock(ext2_filsys fs);
void printInodeInf(ext2_ino_t ino, struct ext2_inode *inode);

unsigned int blockSize(unsigned int);
unsigned int clusterSize(unsigned int, unsigned int);
void printFeatureSets(unsigned int);

#endif 
