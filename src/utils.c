
#include "utils.h"

void free_split(char **cmd_split)
{
    int i = 0;
    while (cmd_split[i])
    {
        free(cmd_split[i]);
        ++i;
    }
    free(cmd_split[i]);
    free(cmd_split);
}

char *ft_str_mem_cpy(char *str)
{
    char *copy;
    if (!(copy = malloc(ft_strlen(str) + 1)))
        return 0;
    ft_strlcpy(copy, str, ft_strlen(str) + 1);
    return copy;
}

char *scan_fd(int fd)
{
    int ret_gnl;
    char *stdin_str = NULL;
    char *current_line;

    while (1)
    {
        ret_gnl = get_next_line(fd, &current_line);
        if (ret_gnl == -1)
        {
            if (stdin_str)
                free(stdin_str);
            return 0;
        }

        if (stdin_str)
        {
            char *buffer = ft_strjoin(stdin_str, current_line);
            free(stdin_str);
            free(current_line);
            if (!buffer)
                return 0;
            stdin_str = buffer;
        }
        else
            stdin_str = current_line;

        if (ret_gnl == 0)
            return stdin_str;
    }
}

void    print_memory_bin(void *data, size_t len)
{
    char *cdata = data;
    for (size_t i = 0; i < len; ++i)
    {
        for (int j = 7; j >= 0; --j)
            ft_printf("%x", cdata[i] >> j & 1);
        ft_printf(" ");
        if (i && i % 8 == 7)
            ft_printf("\n");
    }
    ft_printf("\n");
}

void    print_memory_hex(void *data, size_t len)
{
    unsigned char *cdata = data;
    for (size_t i = 0; i < len; ++i)
    {
        ft_printf("%02x ", cdata[i]);
        if (i && i % 16 == 15)
            ft_printf("\n");
    }
    ft_printf("\n");
}