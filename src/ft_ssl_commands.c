
#include "utils.h"
#include "ft_ssl.h"
#include "help.h"
#include "digest.h"

static const t_command g_commands[] = {
    // Commands
    {"help", "command", parser_help, free_help, exec_help},

    // Message digest commands
    {"md5", "digest", parser_digest, free_digest, exec_md5},
    {"sha256", "digest", parser_digest, free_digest, exec_sha256},
    // Cipher commands
};

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
    const t_command *cmd = find_command(argv[0]);
    if (!cmd)
        ERROR_PRINT(0, "Invalid command '%s'. "
                       "Type \"help\" for a list of available commands.\n",
                       argv[0])

    int ret_val;
    void *p_options;
    if ((ret_val = cmd->parse_token(argv[0], argc - 1, &argv[1], &p_options)) != 1)
        return ret_val;

    ret_val = cmd->exec_cmd(p_options);
    cmd->free_options(p_options);

    return ret_val;
}
