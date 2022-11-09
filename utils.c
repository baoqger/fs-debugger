#include <stdio.h>
#include "utils.h"

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
