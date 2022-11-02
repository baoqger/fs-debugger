#include <errno.h>
#include <stdio.h>
#include <ext2fs/ext2fs.h>
#include <ext2fs/ext3_extents.h>

/* static blk_t backup_superblock = 0; */
void printSuperBlock (ext2_filsys fs);

int main (int argc, char **argv) {
   /* Look in /usr/include/ext2fs/ext3_extents.h /usr/include/ext2fs/ext2fs.h 
      and ext2_fs.h for declarations */
   int i;
   struct struct_ext2_filsys filsys;
   ext2_filsys fs = &filsys;
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
}

void printSuperBlock (ext2_filsys fs) {
   struct ext2_super_block *super = fs->super;
   unsigned int icnt = super->s_inodes_count, 
      bcnt = super->s_blocks_count, 
      feat = super->s_feature_incompat, 
      mtime = super->s_mtime, 
      wtime = super->s_wtime, 
      check = super->s_lastcheck, 
      ficnt = super->s_free_inodes_count, 
      fbcnt = super->s_free_blocks_count,
      bsize = super->s_log_block_size,
      csize = super->s_log_cluster_size,
      gsize = super->s_blocks_per_group;
   printf("Super block information:\n");
   printf("  inodes:%u . blocks:%u . free inodes:%u . free blocks:%u\n",
	  icnt, bcnt, ficnt, fbcnt);
   printf("  block size:%u . cluster size:%u . no. blocks in group:%u\n",
	  bsize, csize, gsize);
   printf("  feature incompat:%x\n", feat);
   printf("  mount time:%u . write time:%u . last check:%u\n",
	  mtime, wtime, check);
   
}
