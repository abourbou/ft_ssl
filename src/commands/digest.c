
#include "digest.h"

int free_and_return_digest(t_digest_options *options)
{
    free_digest(options);
    return -1;
}

int     parser_digest(char *cmd, int nbr_token, char **tokens, void **p_options)
{
    t_digest_options *options;
    if (!(options = malloc(sizeof(t_digest_options))))
        return -1;
    ft_bzero(options, sizeof(t_digest_options));

    if (!(options->files = ft_calloc(sizeof(char*), nbr_token + 1)))
        free_and_return_digest(options);

    int i = 0;
    int nbr_file = 0;
    while (i < nbr_token)
    {
        if (!ft_strcmp(tokens[i], "-p") && !options->pass)
            options->pass = true;
        else if (!ft_strcmp(tokens[i], "-q") && !options->quiet)
            options->quiet = true;
        else if (!ft_strcmp(tokens[i], "-r") && !options->reverse)
            options->reverse = true;
        else if (!ft_strcmp(tokens[i], "-s") && !options->str)
        {
            if (++i == nbr_token)
            {
                free_digest(options);
                ERROR_PRINT(0, "%s: -s must be followed by a string\n", cmd)
            }
            if (!(options->str = ft_str_mem_cpy(tokens[i])))
                free_and_return_digest(options);

        }
        else
        {
            if (!(options->files[nbr_file++] = ft_str_mem_cpy(tokens[i])))
                free_and_return_digest(options);
        }
        ++i;
    }

    *p_options = options;
    return 1;
}

void    free_digest(void *options)
{
    t_digest_options *t_options = options;

    if (t_options->str)
        free(t_options->str);
    if (t_options->files)
    {
        int i = 0;
        while (t_options->files[i])
        {
            free(t_options->files[i]);
            ++i;
        }
        free(t_options->files);
    }
    free(t_options);
}


// Printing

void print_hash(uint8_t* hash, int len)
{
    for (int i = 0; i < len; ++i)
        ft_printf("%02x", hash[i]);
}

void print_hash_newline(uint8_t* hash, int len)
{
    print_hash(hash, len);
    ft_printf("\n");
}

void print_stdin_hash(uint8_t* hash, int len, char *stdin)
{
    if (stdin)
        ft_printf("(\"%s\")= ", stdin);
    else
        ft_printf("(stdin)= ");
    print_hash_newline(hash, len);
}

void print_str_hash(uint8_t* hash, int len, char *algo, char *str, bool rev)
{
    if (!rev)
    {
        ft_printf("%s (\"%s\")= ", algo, str);
        print_hash(hash, len);
    }
    if (rev)
    {
        print_hash(hash, len);
        ft_printf(" \"%s\"", str);
    }
    ft_printf("\n");
}

void print_file_hash(uint8_t* hash, int len, char *algo, char *str, bool rev)
{
    if (!rev)
    {
        ft_printf("%s (%s) = ", algo, str);
        print_hash(hash, len);
    }
    if (rev)
    {
        print_hash(hash, len);
        ft_printf(" %s", str);
    }
    ft_printf("\n");
}

static void stop_to_first_new_line(char *str)
{
    int i = 0;
    while (str[i] && str[i] != '\n')
        ++i;
    if (str[i])
        str[i] = 0;
}

// I/O for digest algorithms
int     exec_digest(char *name_algo, void* options, digest_algo algo, size_t len_hash)
{
    t_digest_options *digest_options = options;
    uint8_t *hash;

    if (!(hash = malloc(len_hash * sizeof(uint8_t))))
        return -1;

    // Read stdin if there is no arguments
    if (!digest_options->pass && !digest_options->str && !digest_options->files[0])
    {
        if (algo(hash, 0, NULL) == -1)
            return free_and_return(-1, hash);

        digest_options->quiet   ? print_hash_newline(hash, len_hash)
                                : print_stdin_hash(hash, len_hash, NULL);
        return free_and_return(1, hash);
    }

    // Pass option
    if (digest_options->pass)
    {
        char *stdin_buffer = scan_fd(0);
        if (!stdin_buffer)
            return free_and_return(-1, hash);

        if (algo(hash, -1, stdin_buffer) == -1)
            return free_and_return(-1, hash);

        if (digest_options->quiet && digest_options->pass)
            ft_printf("%s\n", stdin_buffer);
        stop_to_first_new_line(stdin_buffer);
        digest_options->quiet   ? print_hash_newline(hash, len_hash)
                                : print_stdin_hash(hash, len_hash, stdin_buffer);
        free(stdin_buffer);
    }

    // String option
    if (digest_options->str)
    {
        if (algo(hash, -1, digest_options->str) == -1)
            return free_and_return(-1, hash);

        digest_options->quiet   ? print_hash_newline(hash, len_hash)
                                : print_str_hash(hash, len_hash, name_algo, digest_options->str, digest_options->reverse);
    }

    // Argument files
    int i = -1;
    int fd;
    while (digest_options->files[++i])
    {
        if ((fd = open(digest_options->files[i], O_RDONLY)) == -1)
        {
            ft_fprintf(2, "%s %s: %s\n", name_algo, digest_options->files[i], strerror(errno));
            continue;
        }
        if (algo(hash, fd, NULL) == -1)
            return free_and_return(-1, hash);

        digest_options->quiet   ? print_hash_newline(hash, len_hash)
                                : print_file_hash(hash, len_hash, name_algo, digest_options->files[i], digest_options->reverse);
    }

    free(hash);
    return 1;
}

// Hash a string with a digest algorithm
int digest_hash_str(void *hash, char *str, add_padding_fct add_padding, hash_fct hashing, post_process_fct post_process)
{
    int length_message = ft_strlen(str);
    char *str_with_padding;

    if (!(str_with_padding = malloc(length_message + 128)))
        return -1;
    ft_memcpy(str_with_padding, str, length_message);
    length_message = add_padding(str_with_padding, length_message, length_message);
    hashing(hash, str_with_padding, length_message);
    post_process(hash);
    free(str_with_padding);
    return 1;
}

// Hash a fd with a digest algorithm
int digest_hash_fd(void *hash, int fd, add_padding_fct add_padding, hash_fct hashing, post_process_fct post_process)
{
    char buffer[DIGEST_BUFF_SIZE];
    size_t total_length = 0;
    const size_t size_to_read = DIGEST_BUFF_SIZE - 128;

    while (1)
    {
        size_t length_message = 0;
        int read_val = 0;
        while (length_message < size_to_read)
        {
            read_val = read(fd, buffer + length_message, size_to_read - length_message);
            length_message += read_val;
            if (read_val <= 0)
                break;
        }
        total_length += length_message;

        if (read_val < 0)
            return -1;
        else if (read_val == 0)
        {
            length_message = add_padding(buffer, length_message, total_length);
            hashing((uint64_t*)hash, buffer, length_message);
            post_process(hash);
            return 1;
        }
        else
            hashing((uint64_t*)hash, buffer, length_message);
    }
}

// Generic print help message for the digest algorithms
void digest_print_help(char *name_algo)
{
    ft_printf("Usage:  %s [options] [file...]\n", name_algo);
    ft_printf("%-15s Display this summary\n", "--help");
    ft_printf("%-15s Echo stdin to stdout and compute stdin hash \n", "-p");
    ft_printf("%-15s Quiet mode\n", "-q");
    ft_printf("%-15s Reverse print mode\n", "-r");
    ft_printf("%-15s Compute hash for a string\n", "-s [string]");
}