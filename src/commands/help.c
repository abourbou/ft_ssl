
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

int     exec_help(void* options)
{
    t_help_options* t_options = options;

    if (!t_options->cmd)
    {
        ft_printf("TODO List commands\n");
        //TODO Print list commands of g_commands
        return 1;
    }

    // One argument
    if (!ft_strcmp(t_options->cmd, "--help") ||
        !ft_strcmp(t_options->cmd, "-h"))
    {
        ft_printf("Print informations about the commands\n"
                  "Usage: help [command]\n"
                  "\t--help  Display this summary\n");
        return 1;
    }

    char* argv[2] = {t_options->cmd, "--help"};
    return process_command(2, argv);
}