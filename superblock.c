#include <stdio.h>
#include <math.h>
#include "superblock.h"
#include "utils.h"

void printSuperBlock (ext2_filsys fs) {
   struct ext2_super_block *super = fs->super;
   unsigned int icnt = super->s_inodes_count, 
      bcnt = super->s_blocks_count, 
      feat = super->s_feature_compat, 
      mtime = super->s_mtime, 
      wtime = super->s_wtime, 
      check = super->s_lastcheck, 
      ficnt = super->s_free_inodes_count, 
      fbcnt = super->s_free_blocks_count,
      bsize = super->s_log_block_size,
      csize = super->s_log_cluster_size,
      gsize = super->s_blocks_per_group;
   printf("Super block information:\n");
   printf("\tInode Numbers: %u\n", icnt);
   printf("\tBlock Numbers: %u\n", bcnt);
   printf("\tFree Inode Numbers: %u\n", ficnt);
   printf("\tFree Block Numbers: %u\n", fbcnt);
   printf("\tBlock Size: %u bytes\n", blockSize(bsize));
   printf("\tCluster Size: %u bytes\n", clusterSize(csize, bsize));
   printf("\tBlocks in Group: %u\n", gsize);
   printf("\tfeature incompat:%x\n", feat);
   printFeatureSets(feat);
   printf("\tMount time: "); 
   printFormatTime(mtime);
   printf("\tWrite time: "); 
   printFormatTime(wtime);
   printf("\tLast check time: "); 
   printFormatTime(check);

}

// Block size is 2^(10 + s_log_block_size)
unsigned int blockSize(unsigned int bs) {
    return pow(2, 10 + bs);
}

// Cluster size is 2 ^ s_log_cluster_size blocks if bigalloc is enabled.
unsigned int clusterSize(unsigned int cs, unsigned int bs) {
    return pow(2, 2) * blockSize(bs);
}