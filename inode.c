#include <stdio.h>
#include "inode.h"
#include "utils.h"

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
    printFileType(inode->i_mode);
    printf("\tOwner Uid: %u\n", inode->i_mode);
    printf("\tFile Size: %u bytes\n", inode->i_size);
    printf("\tFile Flag: 0x%x ", inode->i_flags);
    if(inode->i_flags & 0x80000) printf("(Extents)\n"); else printf("\n");

    printf("\n");
    // print i_block layout. It's block map for Ext2/3 and extent tree for Ext4.
    if(inode->i_flags & 0x80000) printf("\tExtent Tree(Ext4):\n"); else printf("\tBlock Map(Ext2)\n");    
    // the length of i_block array is 15.
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
    
    // print interpreted data of above i_block layout as extent tree
    if (inode->i_flags & 0x80000) {
        printf("\n");
        printf("\tExtent Header Content:\n");
        printf("\t\tMagic Number: 0x%04X\n", lowHalf(inode->i_block[0]));
        unsigned int entries_number = highHalf(inode->i_block[0]);
        printf("\t\tNumber of Following Entries: %u(0x%04X)\n", entries_number, entries_number);
        unsigned int max_entries_number = lowHalf(inode->i_block[1]);
        printf("\t\tMax Number of Following Entries: %u(0x%04X)\n", max_entries_number, max_entries_number);
        unsigned int node_depth = highHalf(inode->i_block[1]);
        printf("\t\tCurrent Node Depth in Extent Tree: %u(0x%04X)\n", node_depth, node_depth);
        printf("\t\tA Leaf Node? ");
        if (!node_depth) printf("Yes\n"); else printf("No\n");
        printf("\t\tType of Following Entries: ");
        if (!node_depth) printf("Extent(point to the file's data block)\n"); else printf("Index(point to a block contains more interior nodes in the extent tree)\n");
        // print the data block extent layout
        if (!node_depth) {
            printf("\t\tExtent Layout: \n");
            for (int i = 1; i <= entries_number; i++) {
                unsigned int extent_length = lowHalf(inode->i_block[3*i + 1]);
                unsigned int block_start_low_32 = inode->i_block[3*i + 2];
                unsigned long block_start_high_16 = highHalf(inode->i_block[3*i + 1]);
                unsigned long block_start = (unsigned long)((block_start_high_16 << 32) + block_start_low_32);
                unsigned long block_end = (unsigned long)(block_start + extent_length);
                printf("\t\t\tExtent %d: %lu - %lu\n", i, block_start, block_end);
                printf("\t\t\t\tStart Block Number: %lu(0x%08lX)\n", block_start, block_start);
                printf("\t\t\t\tExtent Length: %u(0x%04X)\n", extent_length, extent_length);
            }
        }
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
