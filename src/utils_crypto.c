
#include "utils_crypto.h"

uint32_t rotate_left(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}

uint32_t rotate_right(uint32_t x, uint32_t n)
{
    return (x >> n) | (x << (32 - n));
}