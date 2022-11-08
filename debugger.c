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
   ext2_ino_t ino = 493118; // fs->super->s_first_ino;
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
    unsigned int i,j,dir;
    unsigned char *ptr;
    struct ext3_extents_header *eh;
    struct ext3_extend_idx *idx;
    struct ext3_extent *ex;

    printf("\tHard Link Count: %u\n", inode->i_links_count); // number of hard links to this inode
    printf("\tData Block Count: %u (4096 bytes per block)\n", inode->i_blocks / 8); 
    printf("\tBlock Array Size: %u\n", EXT2_N_BLOCKS); // i_block[EXT2_N_BLOCKS] for Block map or extent tree
    printf("\tFile Mode: %x\n", inode->i_mode);  // File mode

    printf("\tA directory? ");    
    if(LINUX_S_ISDIR(inode->i_mode)) printf("Yes\n"); else printf("No\n");

    printf("\tOwner Uid: %u\n", inode->i_mode);
    printf("\tFile Size: %u bytes\n", inode->i_size);
    printf("\tFile Flag: 0x%x ", inode->i_flags);
    if(inode->i_flags & 0x80000) printf("(Extents)\n"); else printf("\n");

    if(inode->i_flags & 0x80000) printf("\tExtent Tree(Ext4):\n"); else printf("\tBlock Map(Ext2)\n");    
   
    for(int i = 0; i < 5; i++) {
        printf("\t");
        for(int j = 0; j < 3; j++) {
            pphex(inode->i_block[3*i + j] & 0x000000FF);
            pphex((inode->i_block[3*i + j] & 0x0000FF00) >> 8);
            pphex((inode->i_block[3*i + j] & 0x00FF0000) >> 16);
            pphex((inode->i_block[3*i + j] & 0xFF000000) >> 24);
        }
        printf("\n");
    }
    
    }

void pphex(unsigned char ch) {
    char space[3];
    space[0] = '0';
    space[1] = '0';
    space[2] = 0; // same as space[2] = '\0' to terminate the string

    if((ch % 16) < 10)  space[1] = (char)((ch%16)+'0');  
    if((ch % 16) >= 10) space[1] = (char)(((ch%16)-10)+'A');
    if(((ch >> 4) % 16) < 10)  space[0] = (char)(((ch >> 4) % 16)+'0');
    if(((ch >> 4) % 16) >= 10) space[0] = (char)(((ch >> 4) % 16)-10+'A');
    printf("%s ", space);
    
}


