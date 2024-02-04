
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