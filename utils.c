#include "utils.h"

bool isKthBitSet(int n, int k) {
    if (n & (1 << k)) 
        return 1;
    else 
        return 0;
}
