#ifndef UTILS_H
#define UTILS_H 

#include <stdbool.h>
#include <time.h>

#define BUF_LEN 50
#define FEATURE_LEN 10

bool isKthBitSet(int n, int k);
void printFeatureSets(unsigned int);
void printFormatTime(unsigned int);
void printFileType(unsigned int);
unsigned int highHalf(unsigned int);
unsigned int lowHalf(unsigned int);
#endif 
