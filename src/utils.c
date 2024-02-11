
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

int free_and_return(int val, void *buffer)
{
    free(buffer);
    return val;
}

char *ft_str_mem_cpy(char *str)
{
    char *copy;
    if (!(copy = malloc(ft_strlen(str) + 1)))
        return 0;
    ft_strlcpy(copy, str, ft_strlen(str) + 1);
    return copy;
}

// Scan fd to get the whole string
char *scan_fd(int fd)
{
    char *fd_str = NULL;
    char *buffer;
    char buffer_read[UTILS_BUFF_SIZE + 1];
    int nbr_bytes;

    nbr_bytes = read(fd, buffer_read, UTILS_BUFF_SIZE);
    if (nbr_bytes == -1)
        return 0;
    buffer_read[nbr_bytes] = 0;
    if (!(fd_str = ft_str_mem_cpy(buffer_read)))
        return 0;

    while (1)
    {
        int size = read(fd, buffer_read, UTILS_BUFF_SIZE);
        if (size == -1)
            free_and_return(0, fd_str);
        else if (size == 0)
            return fd_str;
        buffer = ft_strjoin(fd_str, buffer_read);
        free(fd_str);
        if (!buffer)
            return 0;
        fd_str = buffer;
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