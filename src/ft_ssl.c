
#include "utils.h"
#include "ft_ssl.h"

int check_len_cmd(char **cmd)
{
    int i = 0;
    while (cmd[i])
        ++i;

    return i;
}

int main(int argc, char **argv)
{
    // TODO problem when using /n /t in stdin
    // TODO erase "" and '' (maybe more to do ?) in CLI

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

        // Split the command
        if (!(cmd_split = split_cmd(cmd_line)))
        {
            free(cmd_line);
            ERROR_PRINT(EXIT_FAILURE, "Error: Spliting line failed\n")
        }
        // Process command
        int len = check_len_cmd(cmd_split);
        if (len == 0)
            continue;
        ret_val = process_command(len, cmd_split);
        if (ret_val == -1)
        {
            free(cmd_line);
            ERROR_PRINT(-1, "Memory allocation failed in %s\n", cmd_split[0])
        }
        else if (ret_val == 0)
            ft_fprintf(2, "error in %s\n", cmd_split[0]);
    }

    return (0);
}