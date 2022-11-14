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

}


