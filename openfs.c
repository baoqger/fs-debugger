#include <stdio.h>
#include <errno.h>
#include "openfs.h"

int openfs(char *fsname) {
    errcode_t errcode;
    if ((errcode = ext2fs_open(fsname, EXT2_FLAG_RW, 0, 0, unix_io_manager, &fs)) != 0) {
        fprintf(stderr, "Failed to open file syetem: %s\n", fsname);       
        return 1;
    }
    printf("File system %s is opened.\n", fsname);
    return 0;
}
