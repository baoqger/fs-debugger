#include <stdio.h>
#include <ext2fs/ext2fs.h>
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

bool isKthBitSet(int n, int k) {
    if (n & (1 << k)) 
        return 1;
    else 
        return 0;
}

void printFormatTime(unsigned int t) {
    char buf[BUF_LEN] = {0};
    time_t rawtime = (time_t)t;          // simple time of time_t data type
    struct tm *ptm = gmtime(&rawtime);   // broken down time of struct tm data type
    strftime(buf, BUF_LEN, "%c", ptm); // %c: National representation of data and time
    printf("%s\n", buf);
}

/*
 * return two high bytes of one word(32bits)
 * */
unsigned int highHalf(unsigned int i) {
    return (i & 0xFFFF0000) >> 16;
}

/*
 * return two low bytes of one word(32bits)
 * */
unsigned int lowHalf(unsigned int i) {
    return i & 0x0000FFFF;
}

/*
 * print the file type
 * */
void printFileType(unsigned int mode) {
    printf("\tFile Type: ");
    switch(mode & LINUX_S_IFMT) {
        case LINUX_S_IFLNK:
            printf("Symbolic link.\n");
            break;
        case LINUX_S_IFREG:
            printf("Regular file.\n");
            break;
        case LINUX_S_IFDIR:
            printf("Directory.\n");
            break;
        case LINUX_S_IFCHR:
            printf("Character device.\n");
            break;
        case LINUX_S_IFBLK:
            printf("Block device.\n");
            break;
        case LINUX_S_IFIFO:
            printf("Named pipe.\n");
            break;
        case LINUX_S_IFSOCK:
            printf("Socket.\n");
            break;
        default:
            printf("Unknown.\n");
            break;

    }
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
