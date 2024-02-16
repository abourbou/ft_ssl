
#include "utils_crypto.h"

uint32_t rotate_left(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}

uint32_t rotate_right(uint32_t x, uint32_t n)
{
    return (x >> n) | (x << (32 - n));
}

void swap_32bits(void *content, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        uint32_t *p_block_32bits =(uint32_t*)content + i;
        *p_block_32bits = bswap_32(*p_block_32bits);
    }
}
