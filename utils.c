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
    strftime(buf, BUF_LEN, "%c\n", ptm); // %c: National representation of data and time
    printf("%s\n", buf);
}
