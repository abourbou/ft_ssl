
#include "help.h"

int    parser_help(char *cmd, int nbr_token, char **tokens, void **p_options)
{
    (void)cmd;
    if (nbr_token > 1)
        ERROR_PRINT(0, "Usage: help [cmd]\n");

    // Create options
    t_help_options *options;
    if (!(options = malloc(sizeof(t_help_options))))
        return -1;

    if (nbr_token == 0)
        options->cmd = 0;
    else if (!(options->cmd = ft_str_mem_cpy(tokens[0])))
    {
        free(options);
        return -1;
    }
    *p_options = options;
    return 1;
}

void    free_help(void *options)
{
    t_help_options* t_options = options;
    if (t_options->cmd)
        free(t_options->cmd);
    free(t_options);
}

void    print_help_for_help(void)
{
        ft_printf("Print informations about the commands\n"
                  "Usage: help [command]\n"
                  "\t--help  Display this summary\n");
}

int     exec_help(void* options)
{
    t_help_options* t_options = options;

    if (!t_options->cmd)
    {
        print_commands();
        return 1;
    }

    const t_command *cmd = find_command(t_options->cmd);
    if (!cmd)
    {
        ft_fprintf(2, "Error : Invalid command %s\nType \"help\" for a list\n", t_options->cmd);
        return 1;
    }
    cmd->print_help_msg();
    return 1;
}