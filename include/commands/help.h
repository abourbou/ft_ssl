#ifndef HELP_H
# define HELP_H

#include "ft_ssl.h"
#include "utils.h"
#include "libft_core.h"

typedef struct s_help_options {
    char *cmd;
}   t_help_options;

int     parser_help(char *cmd, int nbr_token, char **tokens, void **options);
void    free_help(void *options);
int     exec_help(void* options);

#endif