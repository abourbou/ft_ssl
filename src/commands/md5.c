
#include <math.h>
#include "digest.h"

void init_hash_md5(uint32_t *hash)
{
    static const uint32_t init_hash[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    hash[0] = init_hash[0];
    hash[1] = init_hash[1];
    hash[2] = init_hash[2];
    hash[3] = init_hash[3];
}

static size_t add_padding_and_lengths(char *message, size_t len_input, size_t total_length)
{
    // Add padding
    static char padding[64] = {0};
    padding[0] = 0b10000000;

    size_t remainder_input = len_input % 64;
    size_t padding_size = (remainder_input >= 56)
                        ? 64 - (remainder_input - 56)
                        : 56 - remainder_input;
    ft_memcpy(message + len_input, padding, padding_size);

    // Add length number of bites
    uint64_t lengths_bites = total_length * 8;
    ft_memcpy(message + len_input + padding_size, &lengths_bites, 8);

    return len_input + padding_size + 8;
}

uint32_t *get_K_md5()
{
    static uint32_t K[64];
    static bool initialized = false;

    if (!initialized)
    {
        for (size_t i = 0; i < 64; ++i)
            K[i] = (uint32_t)floor(4294967296 * fabs(sin(i + 1)));
        initialized = true;
    }

    return K;
}

uint32_t* get_S_md5()
{
    static uint32_t S[64];
    static bool initialized = false;

    if (!initialized)
    {
        for (int i = 0; i < 64; ++i)
        {
            int r_i = i % 16;
            if (i / 16 == 0)
                S[i] = (r_i % 4 == 0) ? 7
                       : (r_i % 4 == 1) ? 12
                       : (r_i % 4 == 2) ? 17
                                        : 22;
            else if (i / 16 == 1)
                S[i] = (r_i % 4 == 0) ? 5
                       : (r_i % 4 == 1) ? 9
                       : (r_i % 4 == 2) ? 14
                                        : 20;
            else if (i / 16 == 2)
                S[i] = (r_i % 4 == 0) ? 4
                       : (r_i % 4 == 1) ? 11
                       : (r_i % 4 == 2) ? 16
                                        : 23;
            else
                S[i] = (r_i % 4 == 0) ? 6
                       : (r_i % 4 == 1) ? 10
                       : (r_i % 4 == 2) ? 15
                                        : 21;
        }
        initialized = true;
    }
    return S;
}

void    hash_md5(uint32_t *hash, char *message, size_t message_len)
{
    // Break message into 512 bits chunks
    for (size_t i = 0; i < message_len / 64; ++i)
    {
        uint32_t A = hash[0], B = hash[1], C = hash[2], D = hash[3];
        for (size_t j = 0; j < 64; ++j)
        {
            uint32_t F;
            int g;
            if (j / 16 == 0)
            {
                F = F_MD5(B, C, D);
                g = j;
            }
            else if (j / 16 == 1)
            {
                F = G_MD5(B, C, D);
                g = (5 * j + 1) % 16;
            }
            else if (j / 16 == 2)
            {
                F = H_MD5(B, C, D);
                g = (3 * j + 5) % 16;
            }
            else
            {
                F = I_MD5(B, C, D);
                g = (7 * j) % 16;
            }

            uint32_t M = *(uint32_t*)&message[i * 64 + g * 4];
            uint32_t K = get_K_md5()[j];
            uint32_t S = get_S_md5()[j];
            F = F + A + M + K;
            A = D;
            D = C;
            C = B;
            B = B + rotate_left(F, S);
        }
        hash[0] += A;
        hash[1] += B;
        hash[2] += C;
        hash[3] += D;
    }
}

// Compute md5 hash on the fd or the string
int algo_md5(uint8_t *hash, int fd, char *str)
{
    init_hash_md5((uint32_t*)hash);
    char buffer[MD5_BUFF_SIZE];
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
        hash_md5((uint32_t*)hash, str_with_padding, length_message);
        free(str_with_padding);
        return 1;
    }

    // Fd case : Split the input by blocks
    while (1)
    {
        int length_message = read(fd, buffer, MD5_BUFF_SIZE - 128);
        if (length_message < 0)
            return -1;
        total_length += length_message;

        if (length_message < MD5_BUFF_SIZE - 128)
        {
            length_message = add_padding_and_lengths(buffer, length_message, total_length);
            hash_md5((uint32_t*)hash, buffer, length_message);
            return 1;
        }

        hash_md5((uint32_t*)hash, buffer, length_message);
    }
}

int exec_md5(void* options)
{
    return exec_digest("MD5", options, algo_md5, 16);
}

void print_help_md5(void)
{
    digest_print_help("md5");
}