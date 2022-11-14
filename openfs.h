#ifndef OPENFS_H
#define OPENFS_H 

#include <ext2fs/ext2fs.h>
#include <ext2fs/ext3_extents.h>

extern ext2_filsys fs;

int openfs(char*);


#endif 
