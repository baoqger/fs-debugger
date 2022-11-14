#include <errno.h>
#include <stdio.h>
#include <ext2fs/ext2fs.h>
#include <ext2fs/ext3_extents.h>
#include "fsdb.h"
#include "utils.h"
#include "superblock.h"
#include "inode.h"
#include "command.h"


// when non-zero, this global variable means the user is done using this program
int done = 0;

int main (int argc, char **argv) {
   char *line, *s;

   initialize_readline(); // bind our completer
   
       // loop reading and executing lines until the user quits
    for (; done == 0; ) {
        line = readline("fsdb: ");

        if (!line) {
            break;
        }

        /*
         * Remove leading and trailing whitespace from the line.
         * Then, if there is anything left, add it to the history list
         * and execute it
         * */
        s = stripwhite(line);

        if (*s) {
            add_history(s);
            execute_line(s);
        }
        
        free(line);

    }

    exit(0);

   // ext2_filsys fs = NULL; 
   // errcode_t errcode;
   // char fsname[100] = "/dev/sda5";

   // if (argc < 2) {
   //    printf("Usage: %s /dev/...\n",argv[0]);
   //    exit(0);
   // }

   // strcpy(fsname, argv[1]);
   // errcode = ext2fs_open (fsname, EXT2_FLAG_RW, 0, 0, unix_io_manager, &fs);
   // printf("Return: %d  FLAG:%d\n", (int)errcode, EXT2_FLAG_RW);
   // printSuperBlock(fs); 
   // // Read the first inode
   // ext2_ino_t ino = 125754; // 493118; // fs->super->s_first_ino;
   // struct ext2_inode inode;
   // errcode = ext2fs_read_inode(fs, ino, &inode);
   // printInodeInf(ino, &inode);
}


