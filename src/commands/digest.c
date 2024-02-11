
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
        else if (!ft_strcmp(tokens[i], "-r") && !options->quiet)
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
        ft_printf(" %s", algo);
    }
    ft_printf("\n");
}
