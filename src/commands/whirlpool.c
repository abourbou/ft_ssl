
#include "digest.h"

void init_hash_whirlpool(uint64_t *hash)
{
    static const uint64_t init_hash[8] = {0};

    for (int i = 0; i < 8; ++i)
        hash[i] = init_hash[i];
}

static size_t add_padding_and_lengths(char *message, size_t size_last_chunk, size_t total_length)
{
    // Compute padding size
    size_t remainder_input = size_last_chunk % 32;
    size_t quotient_input = size_last_chunk / 32;
    size_t padding_size = (quotient_input == 0)
                        ? 32 - remainder_input
                        : 64 - remainder_input;
    // Add padding
    static char padding[64] = {0};
    padding[0] = 0b10000000;
    ft_memcpy(message + size_last_chunk, padding, padding_size);

    // Add 32 bytes containing the bite length of the message (in BE)
    char extrabits[24] = {0};
    ft_memcpy(message + size_last_chunk + padding_size, extrabits, 24);
    uint64_t lengths_bites = bswap_64(total_length * 8);
    ft_memcpy(message + size_last_chunk + padding_size + 24, &lengths_bites, 8);

    return size_last_chunk + padding_size + 32;
}

void    hash_whirlpool(uint64_t *hash, char *message, size_t message_len)
{
    (void)hash; (void)message;(void)message_len;
}

int algo_whirlpool(uint8_t *hash, int fd, char *str)
{
    init_hash_whirlpool((uint64_t*)hash);
    char buffer[WHIRLPOOL_BUFF_SIZE];
    size_t total_length = 0;

    // String case
    if (str)
    {
        int length_message = ft_strlen(str);
        char *str_with_padding;
        if (!(str_with_padding = malloc(length_message + 128)))
            return -1;
        ft_memcpy(str_with_padding, str, length_message);
        length_message = add_padding_and_lengths(str_with_padding, length_message, length_message);
        hash_whirlpool((uint64_t*)hash, str_with_padding, length_message);
        swap_32bits(hash, 16);
        free(str_with_padding);
        return 1;
    }

    // Fd case : Split the input by blocks
    while (1)
    {
        int length_message = read(fd, buffer, WHIRLPOOL_BUFF_SIZE - 128);
        if (length_message < 0)
            return -1;
        total_length += length_message;

        if (length_message < WHIRLPOOL_BUFF_SIZE - 128)
        {
            length_message = add_padding_and_lengths(buffer, length_message, total_length);
            hash_whirlpool((uint64_t*)hash, buffer, length_message);
            swap_32bits(hash, 16);
            return 1;
        }
        hash_whirlpool((uint64_t*)hash, buffer, length_message);
    }
}

int exec_whirlpool(void* options)
{
    return exec_digest("WHIRLPOOL", options, algo_whirlpool, 64);
}

void print_help_whirlpool(void)
{
    digest_print_help("whirlpool");
}