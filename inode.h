#ifndef INODE_H
#define INODE_H

#include <ext2fs/ext2fs.h>

void printInodeInf(ext2_filsys, ext2_ino_t ino, struct ext2_inode *inode);
void pphex(unsigned char ch); 

#endif
