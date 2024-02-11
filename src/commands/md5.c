
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

int free_and_return(int val, void *buffer)
{
    free(buffer);
    return val;
}

char *add_padding_and_lengths(char *input_message, size_t *message_len)
{
    char *message;
    size_t len_input = ft_strlen(input_message);

    // Add padding
    size_t remainder_input = len_input % 64;
    char padding[64] = {0};
    padding[0] = 0b10000000;

    size_t padding_size = 0;
    if (remainder_input >= 56)
        padding_size = 64 - (remainder_input - 56);
    else if (remainder_input < 56)
        padding_size = 56 - remainder_input;

    if (!(message = malloc(len_input + padding_size + 8)))
        return 0;
    ft_memcpy(message, input_message, len_input);
    ft_memcpy(message + len_input, padding, padding_size);

    // Add length number of bites
    uint64_t lengths_bites = len_input * 8;

    ft_memcpy(message + len_input + padding_size, &lengths_bites, 8);

    *message_len = len_input + padding_size + 8;
    return message;
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

uint32_t* get_shift_md5()
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

uint32_t rotate_left(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}

int    algo_md5(uint32_t *hash, char *input_message)
{
    char *message;
    size_t  message_len;

    if (!(message = add_padding_and_lengths(input_message, &message_len)))
        return -1;

    init_hash_md5(hash);
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
            uint32_t S = get_shift_md5()[j];
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
    free(message);
    return 1;
}

int exec_md5(void* options)
{
    t_digest_options *digest_options = options;
    uint32_t hash[4];
    char *stdin_buffer;
    // TODO add \n to the string read in scan_fd => solution line below ?
    // TODO Later : Read input by chunk of 1024 bites and hash (usefull for very large files)

    // Read stdin if there is no arguments
    if (!digest_options->pass && !digest_options->str && !digest_options->files[0])
    {
        if (!(stdin_buffer = scan_fd(0)))
            return -1;
        if (algo_md5(hash, stdin_buffer) == -1)
            return (free_and_return(-1, stdin_buffer));

        digest_options->quiet   ? print_hash_newline((uint8_t*)hash, 16)
                                : print_stdin_hash((uint8_t*)hash, 16, NULL);
        free(stdin_buffer);
        return 1;
    }

    // Pass option
    if (digest_options->pass)
    {
        if (!(stdin_buffer = scan_fd(0)))
            return -1;
        if (algo_md5(hash, stdin_buffer) == -1)
            return (free_and_return(-1, stdin_buffer));

        if (digest_options->quiet && digest_options->pass)
            ft_printf("%s\n", stdin_buffer);
        digest_options->quiet   ? print_hash_newline((uint8_t*)hash, 16)
                                : print_stdin_hash((uint8_t*)hash, 16, stdin_buffer);
        free(stdin_buffer);
        stdin_buffer = NULL;
    }

    // String option
    if (digest_options->str)
    {
        if (algo_md5(hash, digest_options->str) == -1)
            return -1;

        digest_options->quiet   ? print_hash_newline((uint8_t*)hash, 16)
                                : print_str_hash((uint8_t*)hash, 16, "MD5", digest_options->str, digest_options->reverse);

    }

    // Argument files
    int i = -1;
    int fd;
    while (digest_options->files[++i])
    {
        if ((fd = open(digest_options->files[i], O_RDONLY)) == -1)
        {
            ft_fprintf(2, "%s: %s\n", digest_options->files[i], strerror(errno));
            continue;
        }
        if (!(stdin_buffer = scan_fd(fd)))
            return -1;
        if (algo_md5(hash, stdin_buffer) == -1)
            return (free_and_return(-1, stdin_buffer));

        digest_options->quiet   ? print_hash_newline((uint8_t*)hash, 16)
                                : print_file_hash((uint8_t*)hash, 16, "MD5", digest_options->files[i], digest_options->reverse);
    }

    return 1;
}
