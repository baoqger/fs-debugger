#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <ext2fs/ext2fs.h>
#include <ext2fs/ext3_extents.h>
#include "debugger.h"
#include "utils.h"

char featureSets[FEATURE_LEN][30] = {
    "Directory Preallocation",  //0x1
    "Image inodes",             //0x2
    "Journal",                  //0x4
    "Extended",                 //0x8
    "Reserved GDT",             //0x10
    "Indexed Directories",      //0x20
    "Lazy BG",                  //0x40
    "Exclude inode",            //0x80
    "Exclude bitmap",           //0x100
    "Sparse Super Block"        //0x200
};


/* static blk_t backup_superblock = 0; */

int main (int argc, char **argv) {
   /* Look in /usr/include/ext2fs/ext3_extents.h /usr/include/ext2fs/ext2fs.h 
      and ext2_fs.h for declarations */
   int i;
   // struct struct_ext2_filsys filsys;
   // ext2_filsys fs = &filsys;
   ext2_filsys fs = NULL; 
   errcode_t errcode;
   char fsname[100] = "/dev/sda5";

   if (argc < 2) {
      printf("Usage: %s /dev/...\n",argv[0]);
      exit(0);
   }

   strcpy(fsname, argv[1]);
   errcode = ext2fs_open (fsname, EXT2_FLAG_RW, 0, 0, unix_io_manager, &fs);
   printf("Return: %d  FLAG:%d\n", (int)errcode, EXT2_FLAG_RW);
   printSuperBlock(fs); 
   // Read the first inode
   ext2_ino_t ino = fs->super->s_first_ino;
   struct ext2_inode inode;
   errcode = ext2fs_read_inode(fs, ino, &inode);
   printInodeInf(ino, &inode);
}

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
   printf("\tBlock Size: %u\n", blockSize(bsize));
   printf("\tCluster Size: %u\n", clusterSize(csize, bsize));
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


// Print feature sets
void printFeatureSets(unsigned int feat) {
    printf("\tFeatures: ");
    for (int i = 0; i < FEATURE_LEN; i++) {
        if (isKthBitSet(feat, i)) 
            printf("%s, ", featureSets[i]);
    }
    printf("\n");
}


void printInodeInf(ext2_ino_t ino, struct ext2_inode  *inode) {
    printf("Inode information for inode %u\n", ino);
}

