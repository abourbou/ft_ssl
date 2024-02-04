
#include <stdbool.h>
#include "libft.h"
#include "utils.h"
#include "ft_ssl.h"

int check_len_cmd(char **cmd)
{
    int i = 0;
    while (cmd[i])
        ++i;

    return i;
}

int keep_getting_entry(char **line)
{
    int i = ft_strlen(*line) - 1;
    while (i >= 0 && strrchr(WHITE_SPACE, (*line)[i]))
        i--;
    if (i > 0 && (*line)[i] == '\\')
    {
        char *buffer = ft_calloc(i + 1, 1);
        if (!buffer)
        {
            free(*line);
            return -1;
        }
        ft_strlcpy(buffer, *line, i + 1);
        free(*line);
        *line = buffer;
        return 1;
    }
    return 0;
}

// Scan stdin as a command
// Return -1 if good, 0 if finished and >=0 if there is an error
int scan_stdin_for_cmd(char **cmd_line)
{
    ft_printf("ft_OpenSSL> ");
    int get_more_line = 0;
    int ret_gnl;
    char *current_line;
    char *buffer;

    *cmd_line = NULL;
    do
    {
        if (get_more_line)
            ft_printf("> ");
        ret_gnl = get_next_line(0, &current_line);

        if (get_more_line)
        {
            buffer = ft_strjoin(*cmd_line, current_line);
            free(*cmd_line);
            free(current_line);
            if (!buffer)
                return -1;
            *cmd_line = buffer;
        }
        else
            *cmd_line = current_line;

        if (ret_gnl == 0)
            free(*cmd_line);
        if (ret_gnl <= 0)
            return ret_gnl;

        get_more_line  = keep_getting_entry(cmd_line);
        if (get_more_line == -1)
            return -1;
    }
    while (get_more_line);

    return 1;
}

int main(int argc, char **argv)
{
    int ret_val;
    if (argc > 1)
    {
        ret_val = process_command(argc - 1, &argv[1]);
        if (ret_val == -1)
            ERROR_PRINT(-1, "Memory allocation failed in %s\n", argv[1])
        else if (ret_val == 0)
            ft_fprintf(2, "error in %s\n", argv[1]);
        return (!ret_val);
    }

    char *cmd_line = NULL;
    char **cmd_split = NULL;
    while(1)
    {
        if (cmd_line)
        {
            free(cmd_line);
            free_split(cmd_split);
        }

        // Scan entry
        ret_val = scan_stdin_for_cmd(&cmd_line);
        if (ret_val < 0)
            ERROR_PRINT(EXIT_FAILURE, "Error: Scanning stdin failed\n")
        else if (ret_val == 0)
            return 0;

        // Split and process the command
        if (!(cmd_split = ft_split_del(cmd_line, WHITE_SPACE)))
            ERROR_PRINT(EXIT_FAILURE, "Error: Spliting line failed\n")
        int len = check_len_cmd(cmd_split);
        if (len == 0)
            continue;
        ret_val = process_command(len, cmd_split);
        if (ret_val == -1)
            ERROR_PRINT(-1, "Memory allocation failed in %s\n", cmd_split[0])
        else if (ret_val == 0)
            ft_fprintf(2, "error in %s\n", cmd_split[0]);
    }

    return (0);
}