
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