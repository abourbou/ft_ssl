
#include "digest.h"

static void init_hash_sha256(uint32_t *hash)
{
    static const uint32_t init_hash[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                          0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    for (int i = 0; i < 8; ++i)
        hash[i] = init_hash[i];
}

static size_t add_padding_sha256(char *message, size_t len_input, size_t total_length)
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
    uint64_t lengths_bites = bswap_64(total_length * 8);
    ft_memcpy(message + len_input + padding_size, &lengths_bites, 8);

    return len_input + padding_size + 8;
}

static uint32_t* get_K_sha256(void)
{
    static uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    return K;
}

static void    hash_sha256(void *vhash, char *message, size_t message_len)
{
    uint32_t *hash = vhash;
    for (size_t nbr_chunk = 0; nbr_chunk < message_len / 64; ++nbr_chunk)
    {
        uint32_t W[64] = {0};
        ft_memcpy(W, &message[nbr_chunk * 64], 64);
        // Convert LE to BE because SHA256 algorithm is on BE
        swap_32bits(W, 64);

        for (int i = 16; i < 64; ++i)
        {
            uint32_t s0 = rotate_right(W[i-15], 7) ^ rotate_right(W[i-15], 18) ^ ((W[i-15]) >> 3);
            uint32_t s1 = rotate_right(W[i-2], 17) ^ rotate_right(W[i-2], 19) ^ (W[i-2] >> 10);
            W[i] = W[i-16] + s0 + W[i-7] + s1;
        }

        // Init values
        uint32_t A = hash[0], B = hash[1], C = hash[2], D = hash[3],
                E = hash[4], F = hash[5], G = hash[6], H = hash[7];

        // Compression function
        for (int i = 0; i < 64; ++i)
        {
            uint32_t s1 = rotate_right(E, 6) ^ rotate_right(E, 11) ^ rotate_right(E, 25);
            uint32_t ch = (E & F) ^ (~E & G);
            uint32_t temp1 = H + s1 +ch + get_K_sha256()[i] + W[i];
            uint32_t s0 = rotate_right(A, 2) ^ rotate_right(A, 13) ^ rotate_right(A, 22);
            uint32_t maj = (A & B) ^ (A & C) ^ (B & C);
            uint32_t temp2 = s0 + maj;
            H = G;
            G = F;
            F = E;
            E = D + temp1;
            D = C;
            C = B;
            B = A;
            A = temp1 + temp2;
        }

        // Update hash
        hash[0] += A;
        hash[1] += B;
        hash[2] += C;
        hash[3] += D;
        hash[4] += E;
        hash[5] += F;
        hash[6] += G;
        hash[7] += H;
    }
}

// Convert to big endian
static void post_process_sha256(void *vhash)
{
    uint8_t *hash = vhash;
    swap_32bits(hash, 8);
}

// Compute sha256 hash on the fd or the string
int algo_sha256(uint8_t *hash, int fd, char *str)
{
    init_hash_sha256((uint32_t*)hash);

    // String case
    if (str)
        return digest_hash_str(hash, str, add_padding_sha256,
                                hash_sha256, post_process_sha256);
    else // Fd case
        return digest_hash_fd(hash, fd, add_padding_sha256,
                                hash_sha256, post_process_sha256);
}

int exec_sha256(void* options)
{
    return exec_digest("SHA256", options, algo_sha256, 32);
}

void print_help_sha256(void)
{
    digest_print_help("sha256");
}