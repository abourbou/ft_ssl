
#include "utils.h"
#include "ft_ssl.h"
#include "help.h"
#include "digest.h"

static const t_command g_commands[] = {
    // Commands
    {"help", "command", parser_help, free_help, exec_help, print_help_for_help},

    // Message digest commands
    {"md5", "digest", parser_digest, free_digest, exec_md5, print_help_md5},
    {"sha256", "digest", parser_digest, free_digest, exec_sha256, print_help_sha256},
    {"whirlpool", "digest", parser_digest, free_digest, exec_whirlpool, print_help_whirlpool},
};

void    print_group_command(char *group_title, char *group_name)
{
    ft_printf("%s\n", group_title);

    unsigned int nbr_commands = sizeof(g_commands) / sizeof(t_command);
    int nbr_commands_print = 0;
    for (size_t i = 0; i < nbr_commands; ++i)
    {
        if (!ft_strcmp(g_commands[i].group_name, group_name))
        {
            nbr_commands_print++;
            ft_printf(" %10s", g_commands[i].name);
        }
        if (nbr_commands_print % 4 == 3 || i == nbr_commands - 1)
            ft_printf("\n");
    }
    ft_printf("\n");
}

void                print_commands(void)
{
    ft_printf("ft_ssl is a program with basic cryptographic functions and utilities.\n\n");
    print_group_command("Standard commands:", "command");
    print_group_command("Message Digest commands:", "digest");
}

const t_command    *find_command(char *cmd_line)
{
    for (size_t i = 0; i < sizeof(g_commands) / sizeof(t_command); ++i)
    {
        if (!ft_strcmp(g_commands[i].name, cmd_line))
            return &g_commands[i];
    }
    return NULL;
}

int process_command(int argc, char **argv)
{
    if (!ft_strcmp(argv[0], "--help") || !ft_strcmp(argv[0], "-h"))
    {
        print_commands();
        return 1;
    }

    const t_command *cmd = find_command(argv[0]);
    if (!cmd)
        ERROR_PRINT(1, "Invalid command '%s'. "
                       "Type \"help\" for a list of available commands.\n",
                       argv[0])

    // Handle help flag
    for (int i = 1; i < argc; ++i)
    {
        if (!ft_strcmp(argv[i], "--help") || !ft_strcmp(argv[i], "-h"))
        {
            cmd->print_help_msg();
            return 1;
        }
    }

    int ret_val;
    void *p_options;
    if ((ret_val = cmd->parse_token(argv[0], argc - 1, &argv[1], &p_options)) != 1)
        return ret_val;

    ret_val = cmd->exec_cmd(p_options);
    cmd->free_options(p_options);

    return ret_val;
}
