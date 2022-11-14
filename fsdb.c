#include <errno.h>
#include <stdio.h>
#include <ext2fs/ext2fs.h>
#include <ext2fs/ext3_extents.h>
#include "fsdb.h"
#include "utils.h"
#include "superblock.h"
#include "inode.h"


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
   ext2_ino_t ino = 125754; // 493118; // fs->super->s_first_ino;
   struct ext2_inode inode;
   errcode = ext2fs_read_inode(fs, ino, &inode);
   printInodeInf(ino, &inode);
}


