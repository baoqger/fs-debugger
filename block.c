#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"

int printBlock(ext2_filsys fs, blk_t blk) {
    unsigned char *buffer = malloc(fs->blocksize);
    errcode_t errcode;
    if ((errcode = io_channel_read_blk(fs->io, blk, 1, buffer)) != 0) {
        fprintf(stderr, "Failed to dump data block %u.\n", blk);
        return 1;
    }
    printFormattedBuffer(buffer, blk);
    free(buffer);
    return 0;
}

int dirBlock(ext2_filsys fs, blk_t blk) {
    unsigned char *buffer = malloc(fs->blocksize);
    errcode_t errcode;
    if ((errcode = io_channel_read_blk(fs->io, blk, 1, buffer)) != 0) {
        fprintf(stderr, "Failed to dump data block %u.\n", blk);
        return 1;
    }
    printDirectoryBlock(buffer, blk);
    free(buffer);
    return 0;
}



void printFormattedBuffer(unsigned char *buffer, blk_t blk) {
    int i,j;

    printf("Contents of block %u:\n", blk);
    for (j = 0; j < 64; j++) {
        printf("0x%04x | ", 16*j);
        // print each charater with 2 hex digits
        // ASCII code's value is in the range of 0 - 127
        for (i = 0; i < 16; i++) {
            if (buffer[16*j + i] < 16) printf("0%x ", buffer[16*j + i]);
            else printf("%x ", buffer[16*j + i]);
        }
        printf("| ");
        // print each character for letters and numbers
        // other characters are represented as "."
        for (i = 0; i < 16; i++) {
            if(buffer[16*j + i] < 32 || buffer[16*j + i] > 127) printf(".");
            else printf("%c", buffer[16*j + i]);
        }
        printf("\n");
    }
}

/*
 * print the data block of directory
 * the data block consisted of entries with type ext4_dir_entry_2 {
 *  __le32 inode;
 *  __le16 rec_len;
 *  __u8   name_len;
 *  __u8   file_type;
 *  char   name[EXT$_NAME_LEN];
 * }
 * */
void printDirectoryBlock(unsigned char *buffer, blk_t blk) {
    char str[128];
    int i;
    ext2_ino_t ino = 0;  // inode number
    unsigned long entry_len = 0; // length(how many bytes) of directory entry
    unsigned long name_len = 0;  // filename string length
    unsigned long type = 0; // file type of 
    unsigned long idx = 0; // index of directory entry
    printf("Contents of directory block %u:\n", blk);
    while(1) {
        ino = buffer[idx] + buffer[idx+1]*(1 << 8) + buffer[idx+2]*(1 << 16) + buffer[idx+3]*(1 << 24); 
        if (ino <= 0) 
            break;
        entry_len = buffer[idx+4] + buffer[idx+5]*(1<<8); 
        name_len = buffer[idx+6];
        type = buffer[idx+7];
        switch(type) {
            case 1: 
                strcpy(str, "File");
                break;
            case 2:
                strcpy(str, "Directory");
                break;
            case 3:
                strcpy(str, "Character Device");
                break;
            case 4:
                strcpy(str, "Block Device");
                break;
            case 5:
                strcpy(str, "Named Pipe");
                break;
            case 6:
                strcpy(str, "Socket");
                break;
            case 7:
                strcpy(str, "Symbolic Link");
                break;
            default:
                strcpy(str, "Unknown");
                break;
        }
        printf("\tInode: %u, Type: %s, Name: ", ino, str);
        for (i = idx+8; i < idx+8+name_len; i++) 
            if (buffer[i] < 32 || buffer[i] > 127) printf(".");
            else printf("%c", buffer[i]);
        printf("\n");
        idx += entry_len;
    } 
    
}
