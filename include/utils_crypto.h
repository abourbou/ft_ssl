#ifndef UTILS_CRYPTO_H
# define UTILS_CRYPTO_H

#include <stdint.h>
#include <stddef.h>
#include <byteswap.h>

/**
 * Cryptographic functions
*/
uint32_t rotate_left(uint32_t x, uint32_t n);
uint32_t rotate_right(uint32_t x, uint32_t n);

/**
 * Convertions
*/

// Swap the bits by fixed size block
void swap_32bits(void *content, size_t len);
void swap_64bits(void *content, size_t len);

#endif