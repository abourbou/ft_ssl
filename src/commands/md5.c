
#include "digest.h"

void    md5_algo(uint32_t *hash, char *data)
{
    (void)hash;
    (void)data;
}

int exec_md5(void* options)
{
    t_digest_options *digest_options = options;
    uint32_t hash[4];
    char *stdin_buffer;
    // TODO Later : Read input and hash at a certain memory limit (usefull for very large files)

    // Read stdin if there is no arguments
    if (!digest_options->pass && !digest_options->str && !digest_options->files[0])
    {
        if (!(stdin_buffer = scan_fd(0)))
            return -1;
        md5_algo(hash, stdin_buffer);

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
        md5_algo(hash, stdin_buffer);

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
        md5_algo(hash, digest_options->str);

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
        md5_algo(hash, stdin_buffer);
        free(stdin_buffer);

        digest_options->quiet   ? print_hash_newline((uint8_t*)hash, 16)
                                : print_file_hash((uint8_t*)hash, 16, "MD5", digest_options->files[i], digest_options->reverse);
    }

    return 1;
}
