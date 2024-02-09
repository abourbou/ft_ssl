
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

int    algo_md5(uint32_t *hash, char *input_message)
{
    char *message;
    size_t  message_len;

    if (!(message = add_padding_and_lengths(input_message, &message_len)))
        return -1;

    init_hash_md5(hash);

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
