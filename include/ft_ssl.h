
#ifndef FT_SSL_H
#define FT_SSL_H

#include "libft_core.h"

// Parse the token and create an option structure for execution
// Return -1 : memory error, 0 : parsing error, 1 : success
typedef int(*token_parser_fct)(char* cmd, int nbr_token, char **tokens, void **p_options);

// Free the option structure
typedef void(*options_free_fct)(void *options);

// Execute the command with options
typedef int(*cmd_exec_fct)(void* options);

typedef struct s_command
{
    // Command description
    char        *name;
    char        *group_name;

    // Usage
    token_parser_fct    parse_token;
    options_free_fct    free_options;
    cmd_exec_fct        exec_cmd;

} t_command;

// Return the index of the command or -1 if not found
const t_command    *find_command(char *cmd_line);
int                 process_command(int argc, char **argv);



#endif