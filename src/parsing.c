
#include "libft.h"
#include "utils.h"
#include "ft_ssl.h"

int keep_getting_entry(char **line)
{
    int i = ft_strlen(*line) - 1;
    while (i >= 0 && ft_strrchr(WHITE_SPACE, (*line)[i]))
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

size_t get_nbr_cmd(char *cmd_line)
{
    size_t len = 0;
    int i = 0;
    while (cmd_line[i])
    {
        while (cmd_line[i] && ft_strchr(WHITE_SPACE, cmd_line[i]))
            ++i;
        if (!cmd_line[i])
            break;
        len++;
        if (cmd_line[i] == '\'')
        {
            ++i;
            while (cmd_line[i] && cmd_line[i] != '\'')
                ++i;
            if (cmd_line[i])
                ++i;
        }
        else if (cmd_line[i] == '\"')
        {
            ++i;
            while (cmd_line[i] && cmd_line[i] != '\"')
                ++i;
            if (cmd_line[i])
                ++i;
        }
        else
        {
            while (cmd_line[i] && cmd_line[i] != '\'' && cmd_line[i] != '\"'
                                && !ft_strchr(WHITE_SPACE, cmd_line[i]))
                ++i;
        }
    }

    return len;
}

char **split_cmd(char *cmd_line)
{
    size_t len = get_nbr_cmd(cmd_line);
    char **cmd_split;
    if (!(cmd_split = ft_calloc(len + 2, sizeof(char*))))
        return 0;

    int i = 0;
    int current_token_id = 0;
    int start_token;
    while (cmd_line[i])
    {
        while (cmd_line[i] && ft_strchr(WHITE_SPACE, cmd_line[i]))
            ++i;
        if (!cmd_line[i])
            break;

        start_token = i;
        if (cmd_line[i] == '\'')
        {
            ++i;
            while (cmd_line[i] && cmd_line[i] != '\'')
                ++i;
            if (cmd_line[i])
                ++i;
        }
        else if (cmd_line[i] == '\"')
        {
            ++i;
            while (cmd_line[i] && cmd_line[i] != '\"')
                ++i;
            if (cmd_line[i])
                ++i;
        }
        else
        {
            while (cmd_line[i] && cmd_line[i] != '\'' && cmd_line[i] != '\"'
                    && !ft_strchr(WHITE_SPACE, cmd_line[i]))
                ++i;
        }
        if (!(cmd_split[current_token_id++] = malloc(i - start_token + 1)))
        {
            free_split(cmd_split);
            return 0;
        }
        ft_strlcpy(cmd_split[current_token_id - 1], &cmd_line[start_token], i - start_token + 1);
    }

    // Erase quotes
    for (size_t i = 0; i < len; ++i)
    {
        if (ft_strlen(cmd_split[i]) > 1 && (cmd_split[i][0] == '\'' || cmd_split[i][0] == '\"'))
        {
            size_t len_token  = ft_strlen(cmd_split[i]);
            ft_memcpy(cmd_split[i], cmd_split[i] + 1, len_token - 2);
            cmd_split[i][len_token - 2] = 0;
        }
    }

    return (cmd_split);
}