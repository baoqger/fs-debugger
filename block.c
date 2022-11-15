#include <stdio.h>
#include <stdlib.h>
#include "block.h"

int printBlock(ext2_filsys fs, blk_t blk) {
    // unsigned char buffer[1050]; 
    unsigned char *buffer = malloc(1024);
    errcode_t errcode;
    if ((errcode = io_channel_read_blk(fs->io, blk, 1, buffer)) != 0) {
        fprintf(stderr, "Failed to dump data block %u.\n", blk);
        return 1;
    }
    printFormattedBuffer(buffer, blk);
    return 0;
}

void printFormattedBuffer(unsigned char *buffer, blk_t blk) {
    int i,j;

    printf("Contents of block %u:\n", blk);
    for (j = 0; j < 64; j++) {
        printf("%5x | ", 16*j);
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
